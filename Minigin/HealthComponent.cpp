#include "MiniginPCH.h"
#include "HealthComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Locator.h"
#include "AudioService.h"

HealthComponent::HealthComponent(const unsigned int& health)
	: m_Lives{ health }
{
}

void HealthComponent::SetLives(const unsigned int& health)
{
	m_Lives = health;
}

void HealthComponent::Die()
{
	//Locator::GetAudio().QueueSound(AudioService::SoundIds::None, true);

	if ((int)m_Lives - 1 < 0)
	{
		//if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == dae::Scene::GameMode::Coop)
		{
			//if (m_pGameObject->GetName() == "Gyaraga")
			//{
			//	if (dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1)->GetComponent<HealthComponent>()->GetLives() <= 0) // or m_Lives?
			//	{
			//		// reset level
			//	}
			//}
			//else
			//{
			//	if (dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<HealthComponent>()->GetLives() <= 0) // or m_Lives?
			//	{
			//		// reset level
			//	}
			//}
		}
		//else
		{
		}
		return;
	}

	m_Lives--;
	m_pGameObject->Notify("PlayerDied");
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}