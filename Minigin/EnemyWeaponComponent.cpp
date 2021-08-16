#include "MiniginPCH.h"
#include "EnemyWeaponComponent.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "CollisionDetectionManager.h"
#include "Scene.h"
#include "EnemyStateManager.h"
#include "EnemyFormationState.h"

EnemyWeaponComponent::EnemyWeaponComponent(bool isAutoFire)
	:m_IsAutoFire{ isAutoFire }
	, m_AutoFireTime{ 0.0f }
	, m_AutoFireTimer{ 2.f }
{
}

void EnemyWeaponComponent::Update()
{
	HandleAutoFire();
	//
	if (m_spBullets.size() <= 0)
	{
		return;
	}
	//check if the bullet is out of the screen
	for (size_t i = 0; i < m_spBullets.size(); i++)
	{
		auto bulletPos = m_spBullets[i]->GetComponent<TransformComponent>()->GetTransform().GetPosition();

		if (bulletPos.y < 0 || bulletPos.y > dae::SceneManager::GetInstance().GetScreenDimensions().y || m_spBullets[i]->GetMarkForDelete())
		{
			CollisionDetectionManager::GetInstance().DeleteSpecificObject(m_spBullets[i]); //delete the bullet from the collisionmanager
			m_spBullets.erase(m_spBullets.begin() + i); //delete the element
		}
	}
}

void EnemyWeaponComponent::Shoot()
{
	CreateBullet();
}

void EnemyWeaponComponent::CreateBullet()
{
	auto position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	const float bulletWidth = 7.0f;
	//choosing which player to shoot at

	std::shared_ptr<GameObject> bullet = std::make_shared<GameObject>("EnemyBullet");
	//add components
	bullet->AddComponent(new TransformComponent(glm::vec2{ position.x + m_pGameObject->GetDimensions().x / 2 - bulletWidth / 2.0f , position.y }));
	bullet->AddComponent(new Texture2DComponent("EnemyBullet.png", 1, false));
	if (StageManager::GetInstance().GetCurrentGameMode() == StageManager::GameMode::Coop)
	{
		const int randNr = rand() % 2;

		if (randNr == 1)
		{
			bullet->AddComponent(new BulletMovementComponent(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetTransform().GetPosition()));
		}
		else
		{
			bullet->AddComponent(new BulletMovementComponent(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1)->GetComponent<TransformComponent>()->GetTransform().GetPosition()));
		}
	}
	else
	{
		bullet->AddComponent(new BulletMovementComponent(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetTransform().GetPosition()));
	}
	bullet->GetComponent<BulletMovementComponent>()->Init();
	//Collision
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(bullet);
	//add to scene and vector
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(bullet);
	m_spBullets.push_back(bullet);
}

void EnemyWeaponComponent::HandleAutoFire()
{
	if (m_IsAutoFire)
	{
		auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);
		if (player && player->GetIsActive())
		{
			m_AutoFireTime += EngineTime::GetInstance().GetDeltaTime();
			//
			if (m_AutoFireTime >= m_AutoFireTimer)
			{
				if (m_pGameObject->GetParent()->GetComponent<EnemyStateManager>())//autoFire is only for formationState
				{
					auto enemyState = m_pGameObject->GetParent()->GetComponent<EnemyStateManager>()->GetState();

					if (dynamic_cast<EnemyFormationState*>(enemyState))
					{
						m_AutoFireTime -= m_AutoFireTime;
						Shoot();
					}
				}
			}
		}
	}
}