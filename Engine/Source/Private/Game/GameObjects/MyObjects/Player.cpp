#include "Game/GameObjects/MyObjects/Player.h"
#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PSCamera.h"
#include "Game/GameObjects/MyObjects/Skull.h"

Player::Player()
{
	isDead = false;
}

void Player::OnStart()
{
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(50.0f) }).lock())
	{
		colRef->type = PECollisionType::PLAYER;
	}
}

void Player::OnTick(float deltaTime)
{
	PWorldObject::OnTick(deltaTime);

	if (const auto& camRef = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock())
	{
		if (isDead)
		{
			if (camRef->transform.position.y >= 50.0f)
			{
				// Fall down to the ground
				camRef->transform.position.y -= 500.0f * deltaTime;

				// Rotate on every axis
				camRef->transform.rotation += 500.0f * deltaTime;
			}
		}
		else
			GetTransform().position = camRef->transform.position;
	}
}

void Player::OnOverlap(const TShared<PWorldObject>& other, const TShared<PSCollision>& otherCol)
{
	if (otherCol->type == PECollisionType::ENEMY)
	{
		isDead = true;
		static bool messagePrinted = false;
		if (!messagePrinted)
		{
			PDebug::Log("GAME OVER\tYOU WERE KILLED BY A SKULL");
			messagePrinted = true;
		}
	}
}