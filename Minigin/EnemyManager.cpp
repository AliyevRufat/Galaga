#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "EnemyStateManager.h"
#include "SceneManager.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "CollisionDetectionManager.h"
#include "HealthComponent.h"
#include "EnemyWeaponComponent.h"
#include "AnimationComponent.h"
#include "SceneManager.h"
#include "TractorBeamComponent.h"
#include "EnemyState.h"
#include "EnemyGoToFormationState.h"
#include "BezierPath.h"

void EnemyManager::Update()
{
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();
	//
	if (m_WaitTime >= 0.0f)
	{
		m_WaitTime -= deltaTime;
		return;
	}
	//
	for (size_t i = 0; i < m_WaitIndices.size(); i++)
	{
		if (m_AmountOfSpawnedEnemies == m_WaitIndices[i])
		{
			m_WaitTime = float(m_WaitTimer);
			//delete element
			auto it = m_WaitIndices.begin() + i;
			m_WaitIndices.erase(std::remove(m_WaitIndices.begin(), m_WaitIndices.end(), *it));
		}
	}
	//
	m_SpawnTimeBee += deltaTime;
	m_SpawnTimeButterfly += deltaTime;
	//
	if (m_SpawnTimeBee >= m_SpawnTimer && m_IndexBees < m_QueuedEnemies.size())
	{
		m_SpawnTimeBee = 0.0f;
		SpawnEnemy(m_QueuedEnemies[m_IndexBees].first, m_QueuedEnemies[m_IndexBees].second.first, m_QueuedEnemies[m_IndexBees].second.second);
		++m_IndexBees;
	}
	if (m_SpawnTimeButterfly >= m_SpawnTimer && m_IndexButterfliesAndBosses < m_SecondQueuedEnemies.size())
	{
		m_SpawnTimeButterfly = 0.0f;
		SpawnEnemy(m_SecondQueuedEnemies[m_IndexButterfliesAndBosses].first, m_SecondQueuedEnemies[m_IndexButterfliesAndBosses].second.first, m_SecondQueuedEnemies[m_IndexButterfliesAndBosses].second.second);
		++m_IndexButterfliesAndBosses;
	}
	//
	if (m_IndexBees == m_QueuedEnemies.size() && m_IndexButterfliesAndBosses == m_SecondQueuedEnemies.size())
	{
		m_AllEnemiesAreSpawned = true;
	}
}

void EnemyManager::QueueEnemy(EnemyType enemyType, int formationRowIndex, int formationIndex, bool secondQueue)
{
	if (secondQueue)
	{
		m_SecondQueuedEnemies.push_back(std::pair(enemyType, std::pair(formationRowIndex, formationIndex)));
	}
	else
	{
		m_QueuedEnemies.push_back(std::pair(enemyType, std::pair(formationRowIndex, formationIndex)));
	}
}

void EnemyManager::SpawnEnemy(EnemyType enemyType, int formationRowIndex, int formationIndex)
{
	++m_AmountOfSpawnedEnemies;
	switch (enemyType)
	{
	case EnemyType::Bee:
		SpawnBee(enemyType, formationRowIndex, formationIndex);
		break;
	case EnemyType::Butterfly:
		SpawnButterfly(enemyType, formationRowIndex, formationIndex);
		break;
	case EnemyType::Boss:
		SpawnBoss(enemyType, formationRowIndex, formationIndex);
		break;
	}
}

void EnemyManager::SpawnBee(EnemyType enemyType, int formationRowIndex, int formationIndex)
{
	const int beeWidth = 48;
	const int beeHeight = 37;
	//
	auto beeEnemy = std::make_shared<GameObject>("Bee", nullptr, glm::vec2(beeWidth, beeHeight));
	beeEnemy->AddComponent(new TransformComponent(glm::vec2(0, 0), glm::vec2(beeWidth, beeHeight)));
	beeEnemy->AddComponent(new Texture2DComponent("Bee.png", 1, true));
	beeEnemy->AddComponent(new AnimationComponent(0.2f, 2, 1, true));
	beeEnemy->AddComponent(new EnemyStateManager(enemyType, formationRowIndex, formationIndex));
	beeEnemy->AddComponent(new EnemyWeaponComponent());
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(beeEnemy);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(beeEnemy);
	m_SpEnemies.push_back(beeEnemy);
}

void EnemyManager::SpawnButterfly(EnemyType enemyType, int formationRowIndex, int formationIndex)
{
	const int butterflyWidth = 48;
	const int butterflyHeight = 37;
	//
	auto butterflyEnemy = std::make_shared<GameObject>("Butterfly", nullptr, glm::vec2(butterflyWidth, butterflyHeight));
	butterflyEnemy->AddComponent(new TransformComponent(glm::vec2(0, 0), glm::vec2(butterflyWidth, butterflyHeight)));
	butterflyEnemy->AddComponent(new Texture2DComponent("Butterfly.png", 1, true));
	butterflyEnemy->AddComponent(new AnimationComponent(0.2f, 2, 1, true));
	butterflyEnemy->AddComponent(new EnemyStateManager(enemyType, formationRowIndex, formationIndex));
	butterflyEnemy->AddComponent(new EnemyWeaponComponent());
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(butterflyEnemy);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(butterflyEnemy);
	m_SpEnemies.push_back(butterflyEnemy);
}

void EnemyManager::SpawnBoss(EnemyType enemyType, int formationRowIndex, int formationIndex)
{
	const int bossWidth = 55;
	const int bossHeight = 59;
	//
	auto bossEnemy = std::make_shared<GameObject>("Boss", nullptr, glm::vec2(bossWidth, bossHeight));
	bossEnemy->AddComponent(new TransformComponent(glm::vec2(0, 0), glm::vec2(bossWidth, bossHeight)));
	bossEnemy->AddComponent(new Texture2DComponent("Boss.png", 1, true));
	bossEnemy->AddComponent(new AnimationComponent(0.2f, 2, 2, true));
	bossEnemy->AddComponent(new EnemyStateManager(enemyType, formationRowIndex, formationIndex));
	bossEnemy->AddComponent(new EnemyWeaponComponent());
	bossEnemy->AddComponent(new HealthComponent(2));
	bossEnemy->AddComponent(new TractorBeamComponent());
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(bossEnemy);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(bossEnemy);
	m_SpEnemies.push_back(bossEnemy);
}

void EnemyManager::ClearEnemies()
{
	m_IndexBees = 0;
	m_IndexButterfliesAndBosses = 0;
	m_SpawnTimeBee = 0.0f;
	m_SpawnTimeButterfly = 0.0f;
	m_QueuedEnemies.clear();
	m_SecondQueuedEnemies.clear();
}

bool EnemyManager::CanDive(EnemyType enemyType) const
{
	switch (enemyType)
	{
	case EnemyType::Bee:
		return m_AmountOfDivingBees < m_MaxAmountOfDivingEnemies;
		break;
	case EnemyType::Butterfly:
		return m_AmountOfDivingButterflies < m_MaxAmountOfDivingEnemies;
		break;
	case EnemyType::Boss:
		return m_AmountOfDivingBosses < m_MaxAmountOfDivingEnemies;
		break;
	}
	return false;
}

void EnemyManager::IncreaseAmountOfDivingEnemies(EnemyType enemyType)
{
	switch (enemyType)
	{
	case EnemyType::Bee:
		++m_AmountOfDivingBees;
		break;
	case EnemyType::Butterfly:
		++m_AmountOfDivingButterflies;
		break;
	case EnemyType::Boss:
		++m_AmountOfDivingBosses;
		break;
	}
}

void EnemyManager::DecreaseAmountOfDivingEnemies(EnemyType enemyType)
{
	switch (enemyType)
	{
	case EnemyType::Bee:
		--m_AmountOfDivingBees;
		break;
	case EnemyType::Butterfly:
		--m_AmountOfDivingButterflies;
		break;
	case EnemyType::Boss:
		--m_AmountOfDivingBosses;
		break;
	}
}

void EnemyManager::IncreaseDifficulty()
{
	++m_EnemyChanceToShoot; // +10%
	++m_MaxAmountOfDivingEnemies; // 1 extra enemy per type can dive at the same time
}

int EnemyManager::GetEnemyChanceToShoot() const
{
	return m_EnemyChanceToShoot;
}

bool EnemyManager::GetAllEnemiesAreSpawned() const
{
	return m_AllEnemiesAreSpawned;
}

void EnemyManager::Wait()
{
	m_WaitIndices.push_back(int(m_QueuedEnemies.size() + m_SecondQueuedEnemies.size()));
}

void EnemyManager::DeleteAllEnemies()
{
	for (auto& spEnemy : m_SpEnemies)
	{
		spEnemy->SetMarkForDelete(true);
		CollisionDetectionManager::GetInstance().DeleteSpecificObject(spEnemy);
	}
	//
	m_IndexBees = 0;
	m_IndexButterfliesAndBosses = 0;
	m_SpawnTimeBee = 0.0f;
	m_SpawnTimeButterfly = 0.0f;
	//
	m_AmountOfDivingBees = 0;
	m_AmountOfDivingButterflies = 0;
	m_AmountOfDivingBosses = 0;
	m_AmountOfSpawnedEnemies = 0;
	//
	m_MaxAmountOfDivingEnemies = 1;
	//
	m_EnemyChanceToShoot = 1;//10%
	m_AllEnemiesAreSpawned = false;
	//
	m_WaitTime = 0.0f;
	//
	m_WaitIndices.clear();
	m_SpEnemies.clear();
	m_QueuedEnemies.clear();
	m_SecondQueuedEnemies.clear();
}

std::pair<BezierPath*, glm::vec2> EnemyManager::GetSpawnPath(EnemyType enemyType, int formationPosIndex, const glm::vec2& endPos)
{
	BezierPath* path = new BezierPath();
	glm::vec2 startPos = glm::vec2(0, 0);
	auto screenWidth = dae::SceneManager::GetInstance().GetScreenDimensions().x;

	if (enemyType == EnemyType::Bee)
	{
		if (formationPosIndex == 4 || formationPosIndex == 5)//first bee wave of first level
		{
			startPos = glm::vec2(350 + 50, -10);

			path->AddCurve({ startPos, glm::vec2(385,screenWidth - 615), glm::vec2(90,screenWidth - 690),glm::vec2(75,screenWidth - 400) }, 30);
			path->AddCurve({ glm::vec2(75,screenWidth - 400), glm::vec2(70,screenWidth - 230) ,glm::vec2(600,750), glm::vec2(endPos.x,endPos.y + 50) }, 30);
			path->AddCurve({ glm::vec2(endPos.x,endPos.y + 50), glm::vec2(endPos.x,endPos.y + 50), endPos,glm::vec2(endPos.x + 30,endPos.y) }, 10);
		}
		else if (formationPosIndex == 2 || formationPosIndex == 3 || formationPosIndex == 6 || formationPosIndex == 7)//second bee wave of first level
		{
			startPos = glm::vec2(425, 0);

			path->AddCurve({ startPos, glm::vec2(425,300), glm::vec2(100,100),glm::vec2(100,300) }, 30);
			path->AddCurve({ glm::vec2(100,300) , glm::vec2(100,500) ,glm::vec2(350,400), glm::vec2(350,350) }, 30);
			path->AddCurve({ glm::vec2(350,350) , glm::vec2(350,320), glm::vec2(350,300) ,endPos }, 10);
		}
		else//third bee wave of first level
		{
			startPos = glm::vec2(425, 0);

			path->AddCurve({ startPos, glm::vec2(425,300), glm::vec2(screenWidth - 100,100),glm::vec2(screenWidth - 100,300) }, 30);
			path->AddCurve({ glm::vec2(screenWidth - 100,300) , glm::vec2(screenWidth - 100,500) ,glm::vec2(screenWidth - 350,400), glm::vec2(screenWidth - 350,350) }, 30);
			path->AddCurve({ glm::vec2(screenWidth - 350,350) , glm::vec2(screenWidth - 350,350), endPos ,endPos }, 10);
		}
	}
	else if (enemyType == EnemyType::Butterfly)
	{
		if (formationPosIndex == 3 || formationPosIndex == 4)//first butterfly wave of first level
		{
			startPos = glm::vec2(350 - 50, -10);

			path->AddCurve({ startPos, glm::vec2(315,screenWidth - 615), glm::vec2(610,screenWidth - 690),glm::vec2(625,screenWidth - 400) }, 30);
			path->AddCurve({ glm::vec2(625,screenWidth - 400), glm::vec2(620,screenWidth - 230) ,glm::vec2(100,750), glm::vec2(endPos.x,endPos.y + 50) }, 30);
			path->AddCurve({ glm::vec2(endPos.x,endPos.y + 50), glm::vec2(endPos.x,endPos.y + 50), endPos,glm::vec2(endPos.x - 30,endPos.y) }, 10);
		}
		else if (formationPosIndex == 2 || formationPosIndex == 5)//second butterfly wave of first level
		{
			startPos = glm::vec2(-50, 560);

			path->AddCurve({ startPos, glm::vec2(680,420), glm::vec2(60,0),glm::vec2(63,350) }, 30);
			path->AddCurve({ glm::vec2(63,350), glm::vec2(60,screenWidth) ,glm::vec2(410,500), glm::vec2(425,300) }, 30);
			path->AddCurve({ glm::vec2(425,300), glm::vec2(425,250), glm::vec2(425,170),endPos }, 10);
		}
		else//third butterfly wave of first level
		{
			startPos = glm::vec2(900, 560);

			path->AddCurve({ startPos, glm::vec2(screenWidth - 680,420), glm::vec2(screenWidth - 60,0),glm::vec2(screenWidth - 63,350) }, 30);
			path->AddCurve({ glm::vec2(screenWidth - 63,350), glm::vec2(screenWidth - 60,screenWidth) ,glm::vec2(screenWidth - 410,500), glm::vec2(screenWidth - 425,300) }, 30);
			path->AddCurve({ glm::vec2(screenWidth - 425,300), glm::vec2(screenWidth - 425,250), glm::vec2(screenWidth - 425,170),endPos }, 10);
		}
	}
	else//Boss path first level
	{
		startPos = glm::vec2(-50, 560);

		path->AddCurve({ startPos, glm::vec2(680,420), glm::vec2(60,0),glm::vec2(63,350) }, 30);
		path->AddCurve({ glm::vec2(63,350), glm::vec2(60,screenWidth) ,glm::vec2(410,500), glm::vec2(425,300) }, 30);
		path->AddCurve({ glm::vec2(425,300), glm::vec2(425,250), glm::vec2(425,170),endPos }, 10);
	}

	std::pair<BezierPath*, glm::vec2> pathAndStartPos(path, startPos);
	return pathAndStartPos;
}