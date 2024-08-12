#pragma once
#include "EngineTypes.h"
#include "Game/PGameEngine.h"

				// Can use shared_from_this() as replacement for "this" keyword for shared pointers
class PObject : public std::enable_shared_from_this<PObject>
{
public:
	PObject();
	virtual ~PObject();

	// Run then the object spawns in
	void Start();

	// Run every frame, passes in deltaTime
	void Tick(float deltaTime);

	//  Run after each tick every frame
	void PostTick(float deltaTime);

	// Mark the object for destroy
	void Destroy();

	// Test if the object is marked for destroy
	bool IsPendingDestroy() const { return m_PendingDestroy; }

	// Set the lifetime of the object to be destroyed after seconds
	void SetLifeTime(float lifeTime) {
		m_LifeTime = lifeTime;
		m_LifeTimeTimer = m_LifeTime;
	}

protected:
	// Run then the object spawns in
	virtual void OnStart() {}

	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime) {}

	//  Run after each tick every frame
	virtual void OnPostTick(float deltaTime) {}

private:
	// If marked for destroy
	bool m_PendingDestroy;

	// If set, destroy object after value of time
	float m_LifeTime;

	// Time before destroy
	float m_LifeTimeTimer;
};