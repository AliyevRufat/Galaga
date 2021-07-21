#include "MiniginPCH.h"
#include "PlayerWeaponComponent.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

PlayerWeaponComponent::PlayerWeaponComponent(SDL_Surface* windowSurface, const int playerWidth)
	:m_MaxBulletCountOnScreen{ 2 }
	, m_CurrentBulletCountOnScreen{ 0 }
	, m_PlayerWidth{ playerWidth }
	, m_Window{ windowSurface }
{
}

void PlayerWeaponComponent::Update()
{
	if (m_spBullets.size() <= 0)
	{
		return;
	}

	for (size_t i = 0; i < m_spBullets.size(); i++) //check if the bullet is out of the screen
	{
		auto bulletPos = m_spBullets[i]->GetComponent<TransformComponent>()->GetTransform().GetPosition();

		if (bulletPos.y < 0 || bulletPos.y > m_Window->h)
		{
			m_spBullets.erase(m_spBullets.begin() + i); //delete the element
			--m_CurrentBulletCountOnScreen;
		}
	}
}

void PlayerWeaponComponent::Shoot()
{
	if (m_CurrentBulletCountOnScreen >= 2)
	{
		return;
	}

	CreateBullet();
}

void PlayerWeaponComponent::CreateBullet()
{
	auto position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	const float bulletWidth = 7.0f;
	//
	std::shared_ptr<GameObject> bullet = std::make_shared<GameObject>("Bullet");
	//add components
	bullet->AddComponent(new TransformComponent(glm::vec2{ position.x + m_PlayerWidth / 2 - bulletWidth / 2.0f , position.y }));
	bullet->AddComponent(new Texture2DComponent("Bullet.png", 1, false));
	bullet->AddComponent(new BulletMovementComponent());
	//add to scene and vector
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(bullet);
	m_spBullets.push_back(bullet);
	//
	++m_CurrentBulletCountOnScreen;
}