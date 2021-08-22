#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/SceneManager.h"
#include "PlayerHealthComponent.h"
#include "BossHealthComponent.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/Locator.h"
#include "EnemyStateManager.h"
#include "EnemyDivingState.h"
#include "AnimationComponent.h"
#include "Texture2DComponent.h"
#include "TractorBeamComponent.h"
#include "EnemyTractorBeamState.h"
#include "ScoreComponent.h"
#include "EnemyFormationState.h"
#include "EnemyGoToBeamState.h"

void CollisionDetectionManager::Update()
{
	if (StageManager::GetInstance().GetCurrentGameMode() == StageManager::GameMode::Coop)
	{
		auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);
		auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);
		if (StageManager::GetInstance().GetIsInMenu() || (player && !player->GetIsActive() && player2 && !player2->GetIsActive()))
		{
			return;
		}
	}
	else
	{
		auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);
		if (StageManager::GetInstance().GetIsInMenu() || (player && !player->GetIsActive()))
		{
			return;
		}
	}

	//if player bullet collides with the enemy
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		for (size_t j = 0; j < m_pOtherEntities.size(); j++)
		{
			if (IsOverlapping(m_pOtherEntityTransforms[i]->GetRect(), m_pOtherEntityTransforms[j]->GetRect()))
			{
				if (m_pOtherEntities[j].first->GetName() == "Bullet")
				{
					IncreasePlayerScore(m_pOtherEntities[j].first, m_pOtherEntities[i].first);
					//
					if ((m_pOtherEntities[i].first->GetName() == "Bee" || m_pOtherEntities[i].first->GetName() == "Butterfly"))
					{
						//set destroy for later for the bullet and the enemy
						Locator::GetAudio().PlaySound("EnemyDeath", true);
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
					else if (m_pOtherEntities[i].first->GetName() != "TractorBeam" && m_pOtherEntities[i].first->GetName() != "Gyaraga" && m_pOtherEntities[i].first->GetName() != "Gyaraga2" && m_pOtherEntities[i].first->GetName() != "Bullet")
					{
						//decrease 1 life of the boss
						auto healthComponent = m_pOtherEntities[i].first->GetComponent<BossHealthComponent>();
						if (healthComponent)
						{
							healthComponent->Die();
						}
						//set destroy for the bullet
						m_pOtherEntities[j].first->SetMarkForDelete(true);
						m_pOtherEntities[j].second = true;
						//
						if (healthComponent && healthComponent->GetLives() <= 0)
						{
							//set destroy for the boss
							m_pOtherEntities[i].first->SetMarkForDelete(true);
							m_pOtherEntities[i].second = true;
							//check if enemy was diving to decrease the amount of diving enemies
							if (m_pOtherEntities[i].first->GetComponent<EnemyStateManager>())
							{
								auto enemyState = m_pOtherEntities[i].first->GetComponent<EnemyStateManager>()->GetState();

								if (dynamic_cast<EnemyDivingState*>(enemyState) || dynamic_cast<EnemyTractorBeamState*>(enemyState) || dynamic_cast<EnemyGoToBeamState*>(enemyState))
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
		if (m_pGyaraga && m_pGyaraga->GetIsActive())
		{
			if (IsOverlapping(m_pGyaragaTransform->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
			{
				if (m_pOtherEntities[i].first->GetName() == "TractorBeam" && !m_pOtherEntities[i].first->GetParent()->GetComponent<TractorBeamComponent>()->GetIsPlayerCaught())
				{
					m_pOtherEntities[i].first->GetParent()->GetComponent<TractorBeamComponent>()->SpawnAFighter(m_pGyaragaTransform->GetTransform().GetPosition());//boss gets the fighter of the player
					m_pGyaraga->GetComponent<PlayerHealthComponent>()->Die();
				}
				else if (m_pOtherEntities[i].first->GetName() != "Bullet" && m_pOtherEntities[i].first->GetName() != "TractorBeam") //everything else except the players bullet and the tractor beam
				{
					if (m_pOtherEntities[i].first->GetName() != "Bee" && m_pOtherEntities[i].first->GetName() != "Butterfly" && m_pOtherEntities[i].first->GetName() != "EnemyBullet" && m_pOtherEntities[i].first->GetName() != "Gyaraga" && m_pOtherEntities[i].first->GetName() != "Gyaraga2")
					{
						m_pOtherEntities[i].first->GetComponent<TractorBeamComponent>()->Clean();//destroy the fighter of the boss if it has one
					}
					//
					AddExplosionEffect(int(i));
					//decrease 1 life of the player and destroy whatever collided with the player
					m_pGyaraga->GetComponent<PlayerHealthComponent>()->Die(true);
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
	}
	if (StageManager::GetInstance().GetCurrentGameMode() == StageManager::GameMode::Coop)
	{
		//if enemy or enemy bullet collides with the player
		for (size_t i = 0; i < m_pOtherEntities.size(); i++)
		{
			if (m_pGyaraga2 && m_pGyaraga2->GetIsActive())
			{
				if (IsOverlapping(m_pGyaragaTransform2->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
				{
					if (m_pOtherEntities[i].first->GetName() == "TractorBeam" && !m_pOtherEntities[i].first->GetParent()->GetComponent<TractorBeamComponent>()->GetIsPlayerCaught())
					{
						m_pOtherEntities[i].first->GetParent()->GetComponent<TractorBeamComponent>()->SpawnAFighter(m_pGyaragaTransform2->GetTransform().GetPosition());//boss gets the fighter of the player
						m_pGyaraga2->GetComponent<PlayerHealthComponent>()->Die();
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
						m_pGyaraga2->GetComponent<PlayerHealthComponent>()->Die(true);
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
	else if (gameObject->GetName() == "Gyaraga2")
	{
		m_pGyaragaTransform2 = transform;
		m_pGyaraga2 = gameObject;
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
			EnemyManager::GetInstance().DeleteSpecificEnemy(m_pOtherEntities[i].first);
			m_pOtherEntities.erase(m_pOtherEntities.begin() + i);
			m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		}
	}
}

void CollisionDetectionManager::DeleteSpecificObject(const std::shared_ptr<GameObject>& object)
{
	if (m_pGyaraga == object)
	{
		m_pGyaraga = nullptr;
		return;
	}
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (m_pOtherEntities[i].first == object)
		{
			m_pOtherEntities.erase(m_pOtherEntities.begin() + i);
			m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
			return;
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

void CollisionDetectionManager::IncreasePlayerScore(const std::shared_ptr<GameObject>& bullet, const std::shared_ptr<GameObject>& gameObject)
{
	auto playerSoreComp = bullet->GetParent()->GetComponent<ScoreComponent>();
	auto enemyStateManager = gameObject->GetComponent<EnemyStateManager>();
	EnemyState* enemyState = nullptr;
	//
	if (enemyStateManager)
	{
		enemyState = enemyStateManager->GetState();
	}
	//
	if (!enemyState)
	{
		return;
	}
	if (dynamic_cast<EnemyFormationState*>(enemyState))
	{
		if (gameObject->GetName() == "Bee")
		{
			playerSoreComp->IncreaseScore(50);
		}
		else if (gameObject->GetName() == "Butterfly")
		{
			playerSoreComp->IncreaseScore(80);
		}
		else
		{
			if (gameObject->GetComponent<BossHealthComponent>()->GetLives() == 1)
			{
				playerSoreComp->IncreaseScore(150);
			}
		}
	}
	else
	{
		if (gameObject->GetName() == "Bee")
		{
			playerSoreComp->IncreaseScore(100);
		}
		else if (gameObject->GetName() == "Butterfly")
		{
			playerSoreComp->IncreaseScore(160);
		}
		else
		{
			if (gameObject->GetComponent<BossHealthComponent>()->GetLives() == 1)
			{
				playerSoreComp->IncreaseScore(400);
			}
		}
	}
}