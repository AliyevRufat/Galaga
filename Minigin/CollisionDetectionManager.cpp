#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/SceneManager.h"
#include "HealthComponent.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/Locator.h"
#include "EnemyStateManager.h"
#include "EnemyDivingState.h"

void CollisionDetectionManager::Update()
{
	//if player bullet collides with the enemy
	for (size_t i = 0; i < m_pOtherEntities.first.size(); i++)
	{
		for (size_t j = 0; j < m_pOtherEntities.first.size(); j++)
		{
			if ((m_pOtherEntities.first[i]->GetName() == "Bee" || m_pOtherEntities.first[i]->GetName() == "Butterfly") && m_pOtherEntities.first[j]->GetName() == "Bullet")
			{
				if (IsOverlapping(m_pOtherEntityTransforms[i]->GetRect(), m_pOtherEntityTransforms[j]->GetRect()))
				{
					m_pOtherEntities.first[i]->SetMarkForDelete(true);
					m_pOtherEntities.first[j]->SetMarkForDelete(true);
					m_pOtherEntities.second[i] = true;
					m_pOtherEntities.second[j] = true;
					//check if enemy was diving
					if (m_pOtherEntities.first[i]->GetComponent<EnemyStateManager>())
					{
						auto enemyState = m_pOtherEntities.first[i]->GetComponent<EnemyStateManager>()->GetState();

						if (dynamic_cast<EnemyDivingState*>(enemyState))
						{
							EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies();
						}
					}
				}
			}
		}
	}
	//if enemy or enemy bullet collides with the player
	for (size_t i = 0; i < m_pOtherEntities.first.size(); i++)
	{
		if (IsOverlapping(m_pGyaragaTransform->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
		{
			if (m_pOtherEntities.first[i]->GetName() == "Bee" || m_pOtherEntities.first[i]->GetName() == "Butterfly" || m_pOtherEntities.first[i]->GetName() == "Boss" || m_pOtherEntities.first[i]->GetName() == "EnemyBullet")
			{
				m_pGyaraga->GetComponent<HealthComponent>()->Die();
				m_pOtherEntities.first[i]->SetMarkForDelete(true);
				m_pOtherEntities.second[i] = true;
				//check if enemy was diving
				if (m_pOtherEntities.first[i]->GetComponent<EnemyStateManager>())
				{
					auto enemyState = m_pOtherEntities.first[i]->GetComponent<EnemyStateManager>()->GetState();

					if (dynamic_cast<EnemyDivingState*>(enemyState))
					{
						EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies();
					}
				}
			}
			else if (m_pOtherEntities.first[i]->GetName() == "Beam")
			{
			}
		}
	}

	DeleteCollidedObjects();
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
		m_pOtherEntities.first.push_back(gameObject);
		m_pOtherEntities.second.push_back(false);
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

void CollisionDetectionManager::DeleteCollidedObjects()
{
	for (size_t i = 0; i < m_pOtherEntities.first.size(); i++)
	{
		if (m_pOtherEntities.second[i])
		{
			m_pOtherEntities.first.erase(m_pOtherEntities.first.begin() + i);
			m_pOtherEntities.second.erase(m_pOtherEntities.second.begin() + i);
			m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		}
	}
}

void CollisionDetectionManager::DeleteSpecificObject(const std::shared_ptr<GameObject>& object)
{
	for (size_t i = 0; i < m_pOtherEntities.first.size(); i++)
	{
		if (m_pOtherEntities.first[i] == object)
		{
			m_pOtherEntities.first.erase(m_pOtherEntities.first.begin() + i);
			m_pOtherEntities.second.erase(m_pOtherEntities.second.begin() + i);
			m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		}
	}
}

void CollisionDetectionManager::ClearCollisions()
{
	for (size_t i = 0; i < m_pOtherEntities.first.size(); i++)
	{
		m_pOtherEntities.first.erase(m_pOtherEntities.first.begin() + i);
		m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		--i;
	}
	m_pGyaragaTransform = nullptr;
	for (size_t i = 0; i < m_pOtherEntityTransforms.size(); i++)
	{
		m_pOtherEntityTransforms[i] = nullptr;
	}
}