#include "Game/GameObjects/PWorldObject.h"
#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PModel.h"

const double pi = 3.14159265358979323846;


TWeak<PModel> PWorldObject::ImportModel(const PString& path)
{
	if (const auto& modelRef = PGameEngine::GetGameEngine()->GetGraphics()->ImportModel(path))
	{
		m_ObjectModels.push_back(modelRef);
		
		return modelRef;
	}
	return {};
}

TWeak<PSCollision> PWorldObject::AddCollision(const PSBox& box, const bool& debug)
{
	// Create hte collision
	const TShared<PSCollision>& newCol = TMakeShared<PSCollision>();
	// Set the position and size
	newCol->box = box;

	// TO DO: DEBUG COLLISION
	if (debug)
	{
		PGameEngine::GetGameEngine()->GetGraphics()->CreateCollisionMesh(newCol);
	}

	// Add the collision to the array
	m_ObjectCollisions.push_back(newCol);

	// Return a weak version
	return newCol;
}

void PWorldObject::TestCollision(const TShared<PWorldObject>& other)
{
	// Looping through this objects collisions
	for (const auto& col : m_ObjectCollisions)
	{
		// Looping through the other objects collisions
		for (const auto& otherCol : other->m_ObjectCollisions)
		{
			if (PSCollision::IsOverlapping(*col, *otherCol))
			{
				OnOverlap(other, otherCol);
			}
		}
	}
}

void PWorldObject::OnPostTick(float deltaTime)
{
	PObject::OnPostTick(deltaTime);

	for (const auto& modelRef : m_ObjectModels)
	{
		modelRef->GetTransform() = GetTransform();
	}

	for (const auto& colRef : m_ObjectCollisions)
	{
		colRef->box.position = GetTransform().position;
	}
}

void PWorldObject::TranslateOffModelRotation(float pointX, float pointZ, float degrees, float& objectX, float& objectZ)
{
	float newX = 0.0f;
	float newZ = 0.0f;

	float finalX = 0.0f;
	float finalZ = 0.0f;

	float radians = degrees * pi / 180;

	// Translate
	newX = objectX - pointX;
	newZ = objectZ - pointZ;

	// Rotate
	finalX = (newX * cos(radians)) - (newZ * sin(radians));
	finalZ = (newX * sin(radians)) + (newZ * cos(radians));

	// Translate to final point
	objectX = finalX + pointX;
	objectZ = finalZ + pointZ;
}