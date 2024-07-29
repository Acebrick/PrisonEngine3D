#version 460 core

in vec3 fColour;
in vec2 fTexCoords;
in vec3 fNormals;

uniform sampler2D colourMap;

struct DirLight {
	vec3 colour;
	vec3 ambient;
	vec3 direction;
	float intensity;
};

#define NUM_DIR_LIGHTS 2;
uniform DirLight dirLights[NUM_DIR_LIGHTS]; // Create a directional light array

// out = going out of the shader into something else
out vec4 finalColour;

void main() {
	// final colour restult for the vertex
	//				   vec3(rgb), alpha
	vec4 result = vec4(vec3(0.0f), 1.0f);

	// Base colour that the object starts at
	vec4 baseColour = texture(colourMap, fTexCoords) * vec4(fColour, 1.0);

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
		vec4 ambientLight = vec4(max(dirLights[i].ambient, baseColour.rgb), 1.0f);

		// Light colour algorithm
		// Adjusts how much colour you can see based on the normal direction
		vec4 lightColour = vec4(dirLights[i].colour, 1.0f);
		lightColour *= colourIntensity;
		lightColour *= dirLights[i].intensity;

		// Add our light values together to get the result
		result += ambientLight + lightColour;
	}
	

	finalColour = result;
}