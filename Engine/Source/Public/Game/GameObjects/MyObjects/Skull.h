#pragma once
#include "Game/GameObjects/PWorldObject.h"

struct PSSpotLight;
struct PSPointLight;

class Skull : public PWorldObject
{
public:
	Skull();

	void SetupPatrol(glm::vec3 patrolPointA, glm::vec3 patrolPointB, float xDir, float zDir);

	void SetIsCircling() { isCircling = true; }

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<PWorldObject>& other, const TShared<PSCollision>& otherCol) override;

private:
	void PatrolSquare(float deltaTime);

	void ChasePlayer(float deltaTime);

	void PatrolPoints(float deltaTime);

	// Variables for skulls circling room
	// TO DO: CLEAN THIS UP IF TIME PERMITS, NOT IMPORTANT
	float skullXDir = 0.0f; // Speed on x axis
	float skullZDir = 0.0f; // Speed on y axis
	bool movingOnX = false; // Determines what direction to move

	float movementSpeed;
	
	bool isAlive;

	// Patrol variables
	// Used for room circling, must be corner with greatest x and z values
	glm::vec3 patrolPointA; 

	// Used with point a for going back and forth between the two points
	glm::vec3 patrolPointB;

	bool isCircling;
	bool isPatrolling;
	float roomLength;

	// Lights attached to the skull
	TWeak<PSSpotLight> m_Eye;
	TWeak<PSPointLight> m_HoverLight;

	// Offsets for lights attached to the skull
	glm::vec3 m_HoverLightOffset;
};