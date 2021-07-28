#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/SceneManager.h"
#include "HealthComponent.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/Locator.h"

void CollisionDetectionManager::Update()
{
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		for (size_t j = 0; j < m_pOtherEntities.size(); j++)
		{
			if (m_pOtherEntities[i]->GetName() == "Bee" && m_pOtherEntities[j]->GetName() == "Bullet")
			{
				if (IsOverlapping(m_pOtherEntityTransforms[i]->GetRect(), m_pOtherEntityTransforms[j]->GetRect()))
				{
					m_pOtherEntities[i]->SetMarkForDelete(true);
					m_pOtherEntities[j]->SetMarkForDelete(true);
					//DeleteCollisionGameObject(m_pOtherEntities[i]);
					//DeleteCollisionGameObject(m_pOtherEntities[j]);
				}
			}
		}
	}

	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (IsOverlapping(m_pGyaragaTransform->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
		{
			if (m_pOtherEntities[i]->GetName() == "Bee" || m_pOtherEntities[i]->GetName() == "Butterfly" || m_pOtherEntities[i]->GetName() == "Boss" || m_pOtherEntities[i]->GetName() == "EnemyBullet")
			{
				m_pGyaraga->GetComponent<HealthComponent>()->Die();
				m_pOtherEntities[i]->SetMarkForDelete(true);
				DeleteCollisionGameObject(m_pOtherEntities[i]);
			}
			else if (m_pOtherEntities[i]->GetName() == "Beam")
			{
			}
		}
	}
}

void CollisionDetectionManager::AddCollisionGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	auto transform = gameObject->GetComponent<TransformComponent>();

	if (gameObject->GetName() == "Gyaraga")
	{
		m_pGyaragaTransform = transform;
		m_pGyaraga = gameObject;
	}
	else
	{
		m_pOtherEntityTransforms.push_back(transform);
		m_pOtherEntities.push_back(gameObject);
	}
}

bool CollisionDetectionManager::IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2)
{
	if ((r1.x + r1.w) < r2.x || (r2.x + r2.w) < r1.x)
	{
		return false;
	}

	if (r1.y > (r2.y + r2.h) || r2.y > (r1.y + r1.h))
	{
		return false;
	}

	return true;
}

void CollisionDetectionManager::DeleteCollisionGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (m_pOtherEntities[i] == gameObject)
		{
			m_pOtherEntities.erase(m_pOtherEntities.begin() + i);
			m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		}
	}
}

void CollisionDetectionManager::ClearCollisions()
{
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		m_pOtherEntities.erase(m_pOtherEntities.begin() + i);
		m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		--i;
	}
	m_pGyaragaTransform = nullptr;
	for (size_t i = 0; i < m_pOtherEntityTransforms.size(); i++)
	{
		m_pOtherEntityTransforms[i] = nullptr;
	}
}