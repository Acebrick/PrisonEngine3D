#version 460 core

in vec3 fColour;
in vec2 fTexCoords;
in vec3 fNormals;
in vec3 fVertPos;
in vec3 fViewPos;

struct Material
{
	sampler2D baseColourMap;
	sampler2D specularMap;
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

#define NUM_DIR_LIGHTS 2 // 2 = Number of available directional lights that can be used
uniform DirLight dirLights[NUM_DIR_LIGHTS]; // Create a directional light array

#define NUM_POINT_LIGHTS 20
uniform PointLight pointLights[NUM_POINT_LIGHTS];

// out = going out of the shader into something else
out vec4 finalColour;

void main() {
	// final colour result for the vertex
	//				   vec3(rgb), alpha
	vec3 result = vec3(0.0f);

	// Base colour that the object starts at
	vec3 baseColour = texture(material.baseColourMap, fTexCoords).rgb * fColour;

	// Specular map value
	vec3 specularColour = texture(material.specularMap, fTexCoords).rgb;

	// Get the view direction
	vec3 viewDir = normalize(fViewPos - fVertPos);

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
		vec3 ambientLight = max(dirLights[i].ambient, baseColour);

		// Light colour algorithm
		// Adjusts how much colour you can see based on the normal direction
		vec3 lightColour = dirLights[i].colour;
		lightColour *= colourIntensity;
		lightColour *= dirLights[i].intensity;

		// Specular power algorithm, calculate the shininesse of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;

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
		vec3 lightColour = pointLights[i].colour;
		lightColour *= diff;
		lightColour *= attenuation;
		lightColour *= pointLights[i].intensity;

		// Specular power algorithm, calculate the shininesse of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;

		// Add our light values together to get the result
		result += (lightColour + specular);
	}

	finalColour = vec4(result, 1.0f);
}