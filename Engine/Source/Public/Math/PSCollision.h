#pragma once
#include "Math/PSBox.h"
#include "EngineTypes.h"

enum class PECollisionType
{
	ALL,
	COLLECTIBLE,
	PLAYER,
	NONE
};

class PMesh;

struct PSCollision
{
	PSCollision()
	{
		box.position = glm::vec3(0.0f);
		box.halfSize = glm::vec3(0.0f);
		type = PECollisionType::ALL;
		tag = "";
	}

	// Determine if 2 collisions are overlapping
	static bool IsOverlapping(const PSCollision& col1, const PSCollision& col2)
	{
		return PSBox::BoxOverlap(col1.box, col2.box);
	}

	// Collision type
	PECollisionType type;

	// Store the debug mesh
	TShared<PMesh> debugMesh;
	 
	// Custom tag for detection
	PString tag;

	// Box bounds
	PSBox box;
};