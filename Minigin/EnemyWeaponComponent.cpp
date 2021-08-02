#include "MiniginPCH.h"
#include "EnemyWeaponComponent.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "CollisionDetectionManager.h"
#include "Scene.h"

EnemyWeaponComponent::EnemyWeaponComponent(const int enemyWidth)
	:m_EnemyWidth{ enemyWidth }
{
}

void EnemyWeaponComponent::Update()
{
	if (m_spBullets.size() <= 0)
	{
		return;
	}

	for (size_t i = 0; i < m_spBullets.size(); i++)//check if the bullet is out of the screen
	{
		auto bulletPos = m_spBullets[i]->GetComponent<TransformComponent>()->GetTransform().GetPosition();

		if (bulletPos.y < 0 || bulletPos.y > dae::SceneManager::GetInstance().GetScreenDimensions().y || m_spBullets[i]->GetMarkForDelete())
		{
			CollisionDetectionManager::GetInstance().DeleteSpecificObject(m_spBullets[i]);
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
	//
	std::shared_ptr<GameObject> bullet = std::make_shared<GameObject>("EnemyBullet");
	//add components
	bullet->AddComponent(new TransformComponent(glm::vec2{ position.x + m_EnemyWidth / 2 - bulletWidth / 2.0f , position.y }));
	bullet->AddComponent(new Texture2DComponent("EnemyBullet.png", 1, false));
	bullet->AddComponent(new BulletMovementComponent(dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetTransform().GetPosition()));
	bullet->GetComponent<BulletMovementComponent>()->Init();
	//Collision
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(bullet);
	//add to scene and vector
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(bullet);
	m_spBullets.push_back(bullet);
}