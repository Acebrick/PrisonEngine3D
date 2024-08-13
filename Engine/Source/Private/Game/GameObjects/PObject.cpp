#include "Game/GameObjects/PObject.h"

PObject::PObject()
{
	m_PendingDestroy = false;
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

	m_LifeTimeTimer += deltaTime;
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