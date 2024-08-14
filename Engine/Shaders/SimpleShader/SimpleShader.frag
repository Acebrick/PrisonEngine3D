#version 460 core

in vec3 fColour;
in vec2 fTexCoords;
in vec3 fNormals;
in vec3 fVertPos;
in vec3 fViewPos;
in mat3 fTBN;

struct Material
{
	sampler2D baseColourMap;
	sampler2D specularMap;
	sampler2D normalMap;
	float shininess;
	float specularStrength;
};

// The material for the shader to interface with our engine material
uniform Material material; 

struct DirLight {
	vec3 colour;
	vec3 ambient;
	vec3 direction;
	float intensity;
};

struct PointLight {
	 vec3 colour;
	 vec3 position;
	 float intensity;
	 float linear;
	 float quadratic;
};

struct SpotLight 
{
	vec3 colour;
	vec3 position;
	vec3 direction;
	float cutOff;
	float linear;
	float quadratic;
	float intensity;
	float outerCutOff;
};

#define NUM_DIR_LIGHTS 1 // 1 = Number of available directional lights that can be used
uniform DirLight dirLights[NUM_DIR_LIGHTS]; // Create a directional light array

#define NUM_POINT_LIGHTS 100
uniform PointLight pointLights[NUM_POINT_LIGHTS];

#define NUM_SPOT_LIGHTS 20
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

// out = going out of the shader into something else
out vec4 finalColour;

void main() {
	// Don't render any fragments with less than 0.1 transparency, not working??
	if (texture(material.baseColourMap, fTexCoords).a < 0.1f)
		discard;

	// final colour result for the vertex
	vec3 result = vec3(0.0f);

	// Base colour that the object starts at
	vec3 baseColour = texture(material.baseColourMap, fTexCoords).rgb * fColour;

	// Specular map value
	vec3 specularColour = texture(material.specularMap, fTexCoords).rgb;

	// Get the view direction
	vec3 viewDir = normalize(fViewPos - fVertPos);

														// NORMAL MAPPING ATTEMPT
	// Normal map value
	vec3 normalColour = texture(material.normalMap, fTexCoords).rgb;
	
	// Opengl reads y/g values reversed
	// This brings it back to normal to correctly read the normal maps, I think???
	// eg. if green is 0		0 * -1 =  0, then  0 + 1 = 1		green is now 1
	// eg. if green is 1		1 * -1 = -1, then -1 + 1 = 0		green is now 0
	// equation -> normalColour.g = normalColour.g * -1.0f + 1.0f;

	// Convert the values from an rgb range (0 to 1) to a normal vector range (-1 to 1)
	normalColour = normalColour * 2.0f - 1.0f;

	// Apply the normalColour to work in tangent space, uncomment to test
	// normalColour = normalize(fTBN * normalColour);

	// Normal maps use colour to represent direction, eg blue facing +1 on Z, green +1 on Y etc...
	// Normal vectors are between -1 an 1, multiplying and adding the 0.5 adjusts the range to 0 and 1 for rgb
	// eg. normalsMin/faceDirection		-1 * 0.5 = -0.5, then -0.5 + 0.5 = 0	0 = No colour
	// eg. normalsMax/faceDirection		 1 * 0.5 =  0.5, then -0.5 + 0.5 = 1	1 = Full colour
	// equation -> normalColour = fNormals * 0.5f + 0.5f;

																	// DIRECTIONAL LIGHTS
	for (int i = 0; i < NUM_DIR_LIGHTS; ++i)
	{
		// Material light direction
		vec3 lightDir = normalize(-dirLights[i].direction);

		// Get the reflection light value
		vec3 reflectDir = reflect(-lightDir, fNormals);

		// How much light should show colour based on direction of normal facing the light
		// dot = get the value of 2 vectors and whether or not they're facing eachother...
		// 1.0 if they are, 0.0 if they're not. eg 100% lit up if directions are opposite, not lit up if the same direction
		float colourIntensity = max(dot(fNormals, lightDir), 0.0f);

		// Ambient algorithm
		// Minimum light value
		vec3 ambientLight = baseColour.rgb * dirLights[i].ambient;

		// Light colour algorithm
		// Adjusts how much colour you can see based on the normal direction
		vec3 lightColour = baseColour * dirLights[i].colour;
		lightColour *= colourIntensity;
		lightColour *= dirLights[i].intensity;

		// Specular power algorithm, calculate the shininesse of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;
		specular *= dirLights[i].intensity;

		// Add our light values together to get the result
		result += (ambientLight + lightColour + specular);
	}

																	// POINT LIGHTS
	for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
	{
		// Light direction from the point light to the vertex
		vec3 lightDir = normalize(pointLights[i].position - fVertPos);

		// Get the reflection light value
		vec3 reflectDir = reflect(-lightDir, fNormals);

		// How much light should show colour based on direction of normal facing the light
		float diff = max(dot(fNormals, lightDir), 0.0f);

		// Distance between the lights position and vertex position
		float distance = length(pointLights[i].position - fVertPos);

		// Actual attenuation calculation
		float attenCalc = 1.0f + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance);

		// Distance that the light can reach
		// Value between 1 and 0 ---- 1 is full light, 0 is no light
		float attenuation = 0.0f;
		
		// Ensure no division by 0
		if (attenCalc != 0.0f)
		{
			attenuation = 1.0f / attenCalc;
		}

		// Light colour algorithm
		// Adjusts how much colour you can see based on the normal direction
		vec3 lightColour = baseColour * pointLights[i].colour;
		lightColour *= diff;
		lightColour *= attenuation;
		lightColour *= pointLights[i].intensity;

		// Specular power algorithm, calculate the shininesse of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;
		specular *= pointLights[i].intensity;

		// Add our light values together to get the result
		result += (lightColour + specular);
	}

																	// SPOT LIGHTS
	for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
	{
		// Light direction from the spot light to the vertex
		vec3 lightDir = normalize(spotLights[i].position - fVertPos);

		// Get the dot product between lightDir and the spot lights direction
		float theta = dot(lightDir, normalize(-spotLights[i].direction));

		// Check if the angle is greater than the spotLights outer cut off
		// If it is, then the fragment is outside the range of the spotlight
		// NOTE: outerCutOff value is the cosine value of the angle not the degrees value...
		// ... Degrees(0) = Cosine(1.0) & Degrees(90) = Cosine(0.0), this is the reason for > and not <
		if (theta > spotLights[i].outerCutOff)
		{
			// Epsilon is the cosine value between the inner and outer cut off
			float epsilon = (spotLights[i].cutOff - spotLights[i].outerCutOff);

			// Determine the intensity of the spot light between the inner and outer cut off
			// Defaulting anything below/above that range to 0 (no light/intensity) and 1 (full light/intensity) respectively
			// Multiply this by the spot lights intensity in the SpotLights struct so that it can still be changed outside of the fragment shader
			float fadeOutIntensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0f, 1.0f) * spotLights[i].intensity;

			// Get the reflection light value
			vec3 reflectDir = reflect(-lightDir, fNormals);

			// How much light should show colour based on direction of normal facing the light
			float diff = max(dot(fNormals, lightDir), 0.0f);

			// Distance between the lights position and vertex position
			float distance = length(spotLights[i].position - fVertPos);

			// Actual attenuation calculation
			float attenCalc = 1.0f + spotLights[i].linear * distance + spotLights[i].quadratic * (distance * distance);

			// Distance that the light can reach
			// Value between 1 and 0 ---- 1 is full light, 0 is no light
			float attenuation = 0.0f;
		
			// Ensure no division by 0
			if (attenCalc != 0.0f)
			{
				attenuation = 1.0f / attenCalc;
			}

			// Light colour algorithm
			// Adjusts how much colour you can see based on the normal direction
			vec3 lightColour = baseColour * spotLights[i].colour;
			lightColour *= diff;
			lightColour *= attenuation;
			lightColour *= fadeOutIntensity;

			// Specular power algorithm, calculate the shininesse of the model
			float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
			vec3 specular = specularColour * specPower;
			specular *= material.specularStrength;
			specular *= spotLights[i].intensity;
			
			// Add our light values together to get the result
			result += (lightColour + specular);
		}
	}


	finalColour = vec4(result, 1.0f);

	
}