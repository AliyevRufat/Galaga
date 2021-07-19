#include "MiniginPCH.h"
#include "HealthComponent.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"
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
	Locator::GetAudio().QueueSound(AudioService::SoundIds::Hurt, true, 50);

	if ((int)m_Lives - 1 < 0)
	{
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == dae::Scene::GameMode::Coop)
		{
			if (m_pGameObject->GetName() == "") //TODO : insert the name of the player1
			{
				if (dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1)->GetComponent<HealthComponent>()->GetLives() <= 0) // or m_Lives?
				{
					// reset level
				}
			}
			else
			{
				if (dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<HealthComponent>()->GetLives() <= 0) // or m_Lives?
				{
					// reset level
				}
			}
		}
		else
		{
		}
		return;
	}

	m_Lives--;
	m_pGameObject->Notify(Event::PlayerDied);
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}