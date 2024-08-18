#pragma once
#include "Game/GameObjects/PWorldObject.h"

class Player : public PWorldObject
{
public:
	Player();

	bool GetIsDead() const { return isDead; }
protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<PWorldObject>& other, const TShared<PSCollision>& otherCol) override;

private:
	bool isDead;
};