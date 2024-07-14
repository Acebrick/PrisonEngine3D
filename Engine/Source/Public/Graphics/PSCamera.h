#pragma once
#include "Math/PSTransform.h"

const double pi = 3.14159265358979323846;

struct PSCamera
{
	PSCamera()
	{
		fov = 70.0f;
		defaultFov = fov;
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
		moveSpeed = 0.1f;
		rotationSpeed = 1.0f;
		thirdPerson = false;
	}

	// Rotate the camera based on the rotation passed in
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f))
	{
		if (glm::length(rotation) != 0.0f)
			rotation = glm::normalize(rotation);

		transform.rotation += rotation * scale * rotationSpeed;

		if (transform.rotation.x < -89.9f)
			transform.rotation.x = -89.9f;

		if (transform.rotation.x > 89.9f)
			transform.rotation.x = 89.9f;

	}

	// Translate the camera based on the translation passed in
	void Translate(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f))
	{
		// Move the input direction forward if required
		glm::vec3 moveDir = transform.Forward() * translation.z;
		moveDir += transform.Right() * translation.x;
		moveDir.y += translation.y;

		if (glm::length(moveDir) != 0.0f)
			moveDir = glm::normalize(moveDir);

		transform.position += moveDir * scale * moveSpeed;
	}

	// Zoom in the fov based on the amount added
	void Zoom(const float& amount)
	{
		fov -= amount;
	}

	// Set the fov to default fov
	void ResetZoom()
	{
		fov = defaultFov;
	}

	// Use the set the fov so thast default fov is updated
	void SetFOV(const float& newFov)
	{
		fov = newFov;
		defaultFov = defaultFov;
	}

	// Turn on/off third person mode
	void ToggleThirdPerson()
	{
		thirdPerson = !thirdPerson;
	}

	glm::vec3 GetPosition()
	{
		return transform.position;
	}

	void RotateAroundObject(float pointX, float pointZ, float degrees)
	{
		float newX = 0.0f;
		float newZ = 0.0f;

		float finalX = 0.0f;
		float finalZ = 0.0f;

		float radians = degrees * pi / 180;

		// Translate
		newX = transform.position.x - pointX;
		newZ = transform.position.z - pointZ;

		// Rotate
		finalX = (newX * cos(radians)) - (newZ * sin(radians));
		finalZ = (newX * sin(radians)) + (newZ * cos(radians));

		// Translate back
		transform.position.x = finalX + pointX;
		transform.position.z = finalZ + pointZ;
	}

	PSTransform transform;
	float fov;
	float defaultFov; // don't change, will auto set based on the fov on initialisation
	float aspectRatio;
	float nearClip;
	float farClip;
	float moveSpeed;
	float rotationSpeed;
	bool thirdPerson;
};