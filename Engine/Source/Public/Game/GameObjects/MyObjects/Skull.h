#pragma once
#include "Game/GameObjects/PWorldObject.h"

struct PSSpotLight;
struct PSPointLight;

class Skull : public PWorldObject
{
public:
	Skull();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

private:
	void PatrolSquare(float deltaTime);

	void ChasePlayer(float deltaTime);

	// Variables for skulls circling room
	// TO DO: CLEAN THIS UP IF TIME PERMITS, NOT IMPORTANT
	float skullXDir = 1.0f; // Speed on x axis
	float skullZDir = 0.0f; // Speed on y axis
	bool movingOnX = false; // Determines what direction to move


	float movementSpeed = 500.0f;
	bool isPatrolling = true;

	// Lights attached to the skull
	TWeak<PSSpotLight> m_Eye;
	TWeak<PSPointLight> m_HoverLight;

	// Offsets for lights attached to the skull
	glm::vec3 leftEyeOffset;
	glm::vec3 rightEyeOffset;
	glm::vec3 m_HoverLightOffset;
};