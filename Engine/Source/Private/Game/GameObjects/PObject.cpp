#include "Game/GameObjects/PObject.h"

PObject::PObject()
{
	m_PendingDestroy = false;
	m_LifeTime = 0.0f;
	m_LifeTimeTimer = 0.0f;

	PDebug::Log("PObject created");
}

PObject::~PObject()
{
	PDebug::Log("PObject destroyed");
}

void PObject::Start()
{
	OnStart();
}

void PObject::Tick(float deltaTime)
{
	OnTick(deltaTime);

	if (m_LifeTime > 0.0f)
	{
		// Countdown the lifetime timer
		m_LifeTimeTimer -= deltaTime;

		// When the timer hits 0 destroy the object
		if (m_LifeTimeTimer <= 0.0f)
			Destroy();
	}
}

void PObject::PostTick(float deltaTime)
{
	OnPostTick(deltaTime);
}

void PObject::Destroy()
{
	m_PendingDestroy = true;

	PGameEngine::GetGameEngine()->DestroyObject(shared_from_this());
}