#pragma once
#include "Game/GameObjects/PWorldObject.h"

class Bludgeon : public PWorldObject
{
public:
	Bludgeon();

	void SwingBludgeon();

	glm::vec3 GetOffset() { return m_BludgeonOffset; }

	bool GetIsSwinging() { return m_IsSwinging; }

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

private:
	glm::vec3 m_BludgeonOffset;

	bool m_IsSwinging;
	float m_StopSwingingTime;
	float m_SwingTime;

	TWeak<PSCollision> collisionBounds;
};