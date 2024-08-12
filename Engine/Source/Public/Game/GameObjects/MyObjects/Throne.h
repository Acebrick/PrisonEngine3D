#pragma once
#include "Game/GameObjects/PWorldObject.h"

class Throne : public PWorldObject
{
public:
	Throne();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<PWorldObject>& other, const TShared<PSCollision>& otherCol) override;
};