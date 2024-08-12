#pragma once

// External Libs
#include <GLM/vec3.hpp>

struct PSLight
{
	PSLight()
	{
		colour = glm::vec3(1.0f);
		intensity = 1.0;
	}
	virtual ~PSLight() = default;

	glm::vec3 colour;
	float intensity;
};

struct PSDirLight : public PSLight
{
	PSDirLight()
	{
		ambient = glm::vec3(0.1f);
		direction = glm::vec3(0.0f);
	}

	glm::vec3 ambient;
	glm::vec3 direction;
};

struct PSPointLight : public PSLight
{
	PSPointLight()
	{
		position = glm::vec3(0.0f);
		linear = 0.045f;
		quadratic = 0.0075f;
	}

	glm::vec3 position;

	// Fall off values for how far the lights can reach
	float linear; 
	float quadratic; 
};

struct PSSpotLight : public PSLight
{
	PSSpotLight()
	{
		position = glm::vec3(0.0f);
		direction = glm::vec3(0.0f, 0.0f, 1.0f);
		innerDegrees = 8.75f;	
		outerDegrees = 17.5f;
		cutOff = glm::cos(glm::radians(innerDegrees));
		outerCutOff = glm::cos(glm::radians(outerDegrees));
		linear = 0.014f;
		quadratic = 0.0007f;
	}

	glm::vec3 position;
	glm::vec3 direction;

	// Stores the cosine value of the width to allow for direct value comparisons in the frag shader
	float cutOff;
	float outerCutOff;

	float linear;
	float quadratic;

	// This is the degrees of the angle of the inner spot light that will always have full intensity
	// Determines the radius of the spot light that will hold maximum intensity
	float innerDegrees;

	// This is the degrees of the angle of the outer spot light that will fade out from 1 to 0 intensity
	// Determines the radius between the edge of the inner radius and the outer cut off
	float outerDegrees;

	// Set the size for the inner circle that always has maximum intensity
	void SetInnerCutOff(float newRadius) 
	{ 
		innerDegrees = newRadius;
		cutOff = glm::cos(glm::radians(innerDegrees));
	}

	// Set the size for the spot light that light will reach 0 intensity
	void SetOuterCutOff(float newRadius) 
	{ 
		outerDegrees = newRadius;
		outerCutOff = glm::cos(glm::radians(outerDegrees));
	}
};