#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/SceneManager.h"
#include "HealthComponent.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/Locator.h"
#include "EnemyStateManager.h"
#include "EnemyDivingState.h"
#include "AnimationComponent.h"
#include "Texture2DComponent.h"
#include "TractorBeamComponent.h"
#include "EnemyTractorBeamState.h"

void CollisionDetectionManager::Update()
{
	//if player bullet collides with the enemy
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		for (size_t j = 0; j < m_pOtherEntities.size(); j++)
		{
			if (IsOverlapping(m_pOtherEntityTransforms[i]->GetRect(), m_pOtherEntityTransforms[j]->GetRect()))
			{
				if (m_pOtherEntities[j].first->GetName() == "Bullet")
				{
					if ((m_pOtherEntities[i].first->GetName() == "Bee" || m_pOtherEntities[i].first->GetName() == "Butterfly"))
					{
						//set destroy for later for the bullet and the enemy
						m_pOtherEntities[i].first->SetMarkForDelete(true);
						m_pOtherEntities[j].first->SetMarkForDelete(true);
						m_pOtherEntities[i].second = true;
						m_pOtherEntities[j].second = true;
						//check if enemy was diving to decrease the amount of diving enemies
						if (m_pOtherEntities[i].first->GetComponent<EnemyStateManager>())
						{
							auto enemyState = m_pOtherEntities[i].first->GetComponent<EnemyStateManager>()->GetState();

							if (dynamic_cast<EnemyDivingState*>(enemyState))
							{
								EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies(m_pOtherEntities[i].first->GetComponent<EnemyStateManager>()->GetEnemyType());
							}
						}
						//explosion
						AddExplosionEffect(int(i));
					}
					else if (m_pOtherEntities[i].first->GetName() == "Boss")
					{
						//decrease 1 life of the boss
						auto healthComponent = m_pOtherEntities[i].first->GetComponent<HealthComponent>();
						healthComponent->Die();
						//set destroy for the bullet
						m_pOtherEntities[j].first->SetMarkForDelete(true);
						m_pOtherEntities[j].second = true;
						//
						if (healthComponent->GetLives() <= 0)
						{
							//set destroy for the boss
							m_pOtherEntities[i].first->SetMarkForDelete(true);
							m_pOtherEntities[i].second = true;
							//check if enemy was diving to decrease the amount of diving enemies
							if (m_pOtherEntities[i].first->GetComponent<EnemyStateManager>())
							{
								auto enemyState = m_pOtherEntities[i].first->GetComponent<EnemyStateManager>()->GetState();

								if (dynamic_cast<EnemyDivingState*>(enemyState) || dynamic_cast<EnemyTractorBeamState*>(enemyState))
								{
									EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies(m_pOtherEntities[i].first->GetComponent<EnemyStateManager>()->GetEnemyType());
								}
							}
							//explosion
							AddExplosionEffect(int(i));
						}
					}
				}
			}
		}
	}
	//if enemy or enemy bullet collides with the player
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (IsOverlapping(m_pGyaragaTransform->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
		{
			if (m_pOtherEntities[i].first->GetName() == "TractorBeam" && !m_pOtherEntities[i].first->GetParent()->GetComponent<TractorBeamComponent>()->GetIsPlayerCaught())
			{
				m_pOtherEntities[i].first->GetParent()->GetComponent<TractorBeamComponent>()->SpawnAFighter(m_pGyaragaTransform->GetTransform().GetPosition());//boss gets the fighter of the player
			}
			else if (m_pOtherEntities[i].first->GetName() != "Bullet" && m_pOtherEntities[i].first->GetName() != "TractorBeam") //everything else except the players bullet and the tractor beam
			{
				if (m_pOtherEntities[i].first->GetName() == "Boss")
				{
					m_pOtherEntities[i].first->GetComponent<TractorBeamComponent>()->Clean();//destroy the fighter of the boss if it has one
				}
				//
				AddExplosionEffect(int(i));
				//decrease 1 life of the player and destroy whatever collided with the player
				m_pGyaraga->GetComponent<HealthComponent>()->Die();
				m_pOtherEntities[i].first->SetMarkForDelete(true);
				m_pOtherEntities[i].second = true;
				//check if enemy was diving
				if (m_pOtherEntities[i].first->GetComponent<EnemyStateManager>())
				{
					auto enemyState = m_pOtherEntities[i].first->GetComponent<EnemyStateManager>()->GetState();

					if (dynamic_cast<EnemyDivingState*>(enemyState) || dynamic_cast<EnemyTractorBeamState*>(enemyState))
					{
						EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies(m_pOtherEntities[i].first->GetComponent<EnemyStateManager>()->GetEnemyType());
					}
				}
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
		m_pOtherEntities.push_back(std::make_pair(gameObject, false));
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
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (m_pOtherEntities[i].second)
		{
			m_pOtherEntities.erase(m_pOtherEntities.begin() + i);
			m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		}
	}
}

void CollisionDetectionManager::DeleteSpecificObject(const std::shared_ptr<GameObject>& object)
{
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (m_pOtherEntities[i].first == object)
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

void CollisionDetectionManager::AddExplosionEffect(int enemyIndex) const
{
	const int offset = 8;
	auto enemyPos = m_pOtherEntities[enemyIndex].first->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto explosion = std::make_shared<GameObject>("Explosion");
	explosion->AddComponent(new TransformComponent(glm::vec2(enemyPos.x - offset, enemyPos.y), glm::vec2(1, 1)));
	explosion->AddComponent(new Texture2DComponent("Explosion.png", 1, true));
	explosion->AddComponent(new AnimationComponent(0.05f, 5, 1, false));
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(explosion);
}