#pragma once
#include "Game/GameObjects/PObject.h"
#include "Math/PSTransform.h"
#include "Math/PSCollision.h"


class PModel;


class PWorldObject : public PObject
{
public:
	PWorldObject() = default;
	virtual ~PWorldObject() = default;
	
	// Import a model and return a weak reference
	TWeak<PModel> ImportModel(const PString& path);

	// Add a collision to the object
	TWeak<PSCollision> AddCollision(const PSBox& box, const bool& debug = false);

	// Get the objects transform
	PSTransform& GetTransform() { return m_Transform; }

	// Run a test to see if another word is overlapping
	void TestCollision(const TShared<PWorldObject>& other);

	// Does the object have collisions
	bool HasCollisions() const { return m_ObjectCollisions.size() > 0; }

	// Don't store these as a reference for memory
	TArray<TShared<PSCollision>> GetCollisions() const { return m_ObjectCollisions; }
	
	void TranslateOffModelRotation(float pointX, float pointZ, float degrees, float& objectX, float& objectZ);

protected:
	void OnPostTick(float deltaTime) override;

	// On collision overlap
	// Detect every frame a collision is overlapped
	virtual void OnOverlap(const TShared<PWorldObject>& other, const TShared<PSCollision>& otherCol) {}

private:
	// Transform in world space
	PSTransform m_Transform;

	// Store any models attached to this object
	TArray<TShared<PModel>> m_ObjectModels;

	// Store the collisions for the model
	TArray<TShared<PSCollision>> m_ObjectCollisions;
};