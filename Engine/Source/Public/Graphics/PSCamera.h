#pragma once
#include "Math/PSTransform.h"
#include "Game/PGameEngine.h"
#include "Game/GameObjects/MyObjects/Bludgeon.h"

struct PSCamera
{
	PSCamera()
	{
		fov = 70.0f;
		defaultFov = fov;
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
		moveSpeed = 40.0f;
		rotationSpeed = 1.5f;
	}

	// Rotate the camera based on the rotation passed in
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f))
	{
		float oldYRot = transform.rotation.y;

		if (glm::length(rotation) != 0.0f)
			rotation = glm::normalize(rotation);

		transform.rotation += rotation * scale * rotationSpeed;

		if (transform.rotation.x < -89.9f)
			transform.rotation.x = -89.9f;

		if (transform.rotation.x > 89.9f)
			transform.rotation.x = 89.9f;

		if (oldYRot != transform.rotation.y)
		{
			if (const auto& bludgeonRef = PGameEngine::GetGameEngine()->GetBludgeon().lock())
			{
					bludgeonRef->TranslateOffModelRotation(transform.position.x, transform.position.z, -(transform.rotation.y - oldYRot),
						bludgeonRef->GetTransform().position.x, bludgeonRef->GetTransform().position.z);
			}
		}
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

		if (const auto& bludgeonRef = PGameEngine::GetGameEngine()->GetBludgeon().lock())
		{
			if (moveDir != glm::vec3(0.0f))
			{
				bludgeonRef->GetTransform().position.x += moveDir.x * scale.x * moveSpeed;
				bludgeonRef->GetTransform().position.z += moveDir.z * scale.z * moveSpeed;
			}
		}
		PDebug::Log( "\tX: " + std::to_string(transform.position.x) +
			"\tY: " + std::to_string(transform.position.y) +
			"\tZ: " + std::to_string(transform.position.z));
		//PDebug::Log( "\tX: " + std::to_string(transform.Forward().x) +
		//	"\tY: " + std::to_string(transform.Forward().y) +
		//	"\tZ: " + std::to_string(transform.Forward().z));
		
		// Apply constraint to the movement on the y axis
		transform.position.y = 350.0f;
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

	PSTransform transform;
	float fov;
	float defaultFov; // don't change, will auto set based on the fov on initialisation
	float aspectRatio;
	float nearClip;
	float farClip;
	float moveSpeed;
	float rotationSpeed;
};