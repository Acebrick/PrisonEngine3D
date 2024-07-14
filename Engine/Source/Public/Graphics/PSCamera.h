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
	void ToggleThirdPerson(const PSTransform& object, const glm::vec3 cameraOffset)
	{
		thirdPerson = !thirdPerson;

		if (thirdPerson)
		{
			// Move to objects position and copy its rotation
			transform.rotation = object.rotation;
			transform.position = object.position;

			// Offset the camera
			transform.position.x += cameraOffset.x;
			transform.position.y += cameraOffset.y;
			transform.position.z += cameraOffset.z;

			// Move around the object relative to how much it has rotated to face the same direction
			TranslateOffModelRotation(
				object.position.x,
				object.position.z,
				-object.rotation.y);

			PDebug::Log("Third person mode on");
		}
		else
			PDebug::Log("Third person mode off");
	}

	// Have the camera translate (looks like rotating) around the target object... 
	// ... in this case the tank to maintain third person perspective from behind the model
	void TranslateOffModelRotation(float pointX, float pointZ, float degrees)
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