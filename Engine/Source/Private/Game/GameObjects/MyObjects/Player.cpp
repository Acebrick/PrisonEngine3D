#include "Game/GameObjects/MyObjects/Player.h"
#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PSCamera.h"

Player::Player()
{
	
}

void Player::OnStart()
{
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(10.0f) }).lock())
	{
		colRef->type = PECollisionType::PLAYER;
	}
}

void Player::OnTick(float deltaTime)
{
	PWorldObject::OnTick(deltaTime);

	if (const auto& camRef = PGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock())
	{
		GetTransform().position = camRef->transform.position;
	}

	/*PDebug::Log("X: " + std::to_string(GetTransform().position.x) + 
				"Y: " + std::to_string(GetTransform().position.y) + 
				"Z: " + std::to_string(GetTransform().position.z));*/
}
