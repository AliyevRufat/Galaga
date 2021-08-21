#include "MiniginPCH.h"
#include "BossHealthComponent.h"
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

BossHealthComponent::BossHealthComponent(const unsigned int& health)
	: m_Lives{ health }
{
}

void BossHealthComponent::Die()
{
	Locator::GetAudio().PlaySound("EnemyDeath2", true);

	if (m_Lives > 0)
	{
		m_Lives--;
	}

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

const unsigned int& BossHealthComponent::GetLives() const
{
	return m_Lives;
}