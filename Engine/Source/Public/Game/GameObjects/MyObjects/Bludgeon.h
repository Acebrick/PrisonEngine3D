#pragma once
#include "Game/GameObjects/PWorldObject.h"

class Bludgeon : public PWorldObject
{
public: 
	Bludgeon();

	void SwingBludgeon();

	glm::vec3 GetOffset() { return m_BludgeonOffset; }

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

private:
	glm::vec3 m_BludgeonOffset;
	glm::vec3 m_DefaultRotation;
	bool isSwinging;
};