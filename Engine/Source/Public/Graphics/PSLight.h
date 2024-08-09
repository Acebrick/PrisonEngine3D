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
		ambient = glm::vec3(0.01f);
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

		// Technically the angle and not the radius, but is used to determine the radius
		radius = 12.5f;

		// The variable radius is storing the cosine value of the width to allow for direct value comparisons in the frag shader
		// Radius is the angle between the spotlights direction and the lights direction (direction of triangle face)
		cutOff = glm::cos(glm::radians(radius));
		linear = 0.045f;
		quadratic = 0.0075f;
	}

	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float linear;
	float quadratic;
	float radius;

	// Allows for manually adjusting the spotlights size during runtime
	void SetCutOff(float newRadius) { cutOff = glm::cos(glm::radians(newRadius)); }
};