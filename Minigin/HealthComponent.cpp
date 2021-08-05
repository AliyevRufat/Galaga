#include "MiniginPCH.h"
#include "HealthComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Locator.h"
#include "AudioService.h"
#include "AnimationComponent.h"

HealthComponent::HealthComponent(const unsigned int& health)
	: m_Lives{ health }
{
}

void HealthComponent::Die()
{
	if (m_pGameObject->GetName() == "Gyaraga")
	{
		m_pGameObject->Notify("PlayerDied");
	}
	else if (m_pGameObject->GetName() == "Boss")
	{
		if (m_Lives > 1)
		{
			m_pGameObject->GetComponent<AnimationComponent>()->SetCurrentRowIndex(1);
		}
		else
		{
		}
	}
	//
	m_Lives--;
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}