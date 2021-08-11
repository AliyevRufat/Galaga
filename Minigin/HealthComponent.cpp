#include "MiniginPCH.h"
#include "HealthComponent.h"
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

HealthComponent::HealthComponent(const unsigned int& health)
	: m_Lives{ health }
{
}

void HealthComponent::Die()
{
	if (m_Lives > 0)
	{
		m_Lives--;
	}
	//
	if (m_pGameObject->GetName() == "Gyaraga")
	{
		m_pGameObject->Notify("PlayerDied");
		m_pGameObject->SetIsActive(false);
		CollisionDetectionManager::GetInstance().DeleteSpecificObject(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0));
		//death animation
		auto enemyPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		auto explosion = std::make_shared<GameObject>("PlayerDeath");
		explosion->AddComponent(new TransformComponent(glm::vec2(enemyPos.x - m_pGameObject->GetDimensions().x / 2, enemyPos.y + m_pGameObject->GetDimensions().y / 10), glm::vec2(1, 1)));
		explosion->AddComponent(new Texture2DComponent("PlayerDeath.png", 1, true));
		explosion->AddComponent(new AnimationComponent(0.10f, 4, 1, false));
		dae::SceneManager::GetInstance().GetCurrentScene()->Add(explosion);
	}
	else if (m_pGameObject->GetName() == "Boss")
	{
		if (m_Lives == 1)
		{
			m_pGameObject->GetComponent<AnimationComponent>()->SetCurrentRowIndex(1);
		}
		else
		{
			auto tractorBeamComp = m_pGameObject->GetComponent<TractorBeamComponent>();
			if (tractorBeamComp)
			{
				tractorBeamComp->Clean();
			}
		}
	}
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}