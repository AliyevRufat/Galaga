#include "MiniginPCH.h"
#include "BossHealthComponent.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/Locator.h"
#include "../AliEngine/AudioService.h"
#include "AnimationComponent.h"
#include "TractorBeamComponent.h"
#include "EnemyManager.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/Texture2DComponent.h"
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