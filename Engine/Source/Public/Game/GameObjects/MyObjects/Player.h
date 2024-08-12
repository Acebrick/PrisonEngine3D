#pragma once
#include "Game/GameObjects/PWorldObject.h"

class Player : public PWorldObject
{
public:
	Player();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;
};