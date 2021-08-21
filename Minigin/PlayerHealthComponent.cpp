#include "MiniginPCH.h"
#include "PlayerHealthComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Locator.h"
#include "AudioService.h"
#include "AnimationComponent.h"
#include "TractorBeamComponent.h"
#include "EnemyManager.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "CollisionDetectionManager.h"

PlayerHealthComponent::PlayerHealthComponent(const unsigned int& health)
	: m_Lives{ health }
	, m_IsGameOver{ false }
{
}

void PlayerHealthComponent::Die(bool explode)
{
	Locator::GetAudio().PlaySound("PlayerDeath", true);

	if (m_Lives > 0)
	{
		m_Lives--;
		if (m_Lives == 0)
		{
			m_IsGameOver = true;
		}
	}
	//
	if (m_pGameObject->GetName() == "Gyaraga" || m_pGameObject->GetName() == "Gyaraga2")
	{
		m_pGameObject->Notify("PlayerDied");
		m_pGameObject->SetIsActive(false);
		if (m_pGameObject->GetName() == "Gyaraga")
		{
			CollisionDetectionManager::GetInstance().DeleteSpecificObject(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0));
		}
		else
		{
			CollisionDetectionManager::GetInstance().DeleteSpecificObject(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1));
		}
		if (explode)
		{
			//death animation
			const int offsetX = 2;
			const int offsetY = 12;
			auto playerPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			auto explosion = std::make_shared<GameObject>("PlayerDeath");
			explosion->AddComponent(new TransformComponent(glm::vec2(playerPos.x - m_pGameObject->GetDimensions().x / 2 - offsetX, playerPos.y + m_pGameObject->GetDimensions().y / 10 - offsetY), glm::vec2(1, 1)));
			explosion->AddComponent(new Texture2DComponent("PlayerDeath.png", 1, true));
			explosion->AddComponent(new AnimationComponent(0.10f, 4, 1, false));
			dae::SceneManager::GetInstance().GetCurrentScene()->Add(explosion);
		}
		//
		if (m_IsGameOver)
		{
			StageManager::GetInstance().SetIsGameOver(true, m_pGameObject->GetName());
		}
		else
		{
			StageManager::GetInstance().SetIsPlayerDead(true, m_pGameObject->GetName());
		}
	}
}

const unsigned int& PlayerHealthComponent::GetLives() const
{
	return m_Lives;
}

bool PlayerHealthComponent::GetIsDead() const
{
	return m_IsGameOver;
}