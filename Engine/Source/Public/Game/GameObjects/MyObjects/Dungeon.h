#pragma once
#include "Game/GameObjects/PWorldObject.h"

class Dungeon : public PWorldObject
{
public:
	Dungeon() = default;

protected:
	void OnStart() override;
};