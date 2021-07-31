#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "EnemyStateManager.h"
#include "SceneManager.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "CollisionDetectionManager.h"

void EnemyManager::Update()
{
	m_SpawnTime += EngineTime::GetInstance().GetDeltaTime();

	if (m_SpawnTime >= m_SpawnTimer && m_Index < m_QueuedEnemies.size())
	{
		m_SpawnTime -= m_SpawnTime;
		SpawnEnemy(m_QueuedEnemies[m_Index].first, m_QueuedEnemies[m_Index].second);
		++m_Index;
	}
}

void EnemyManager::QueueEnemy(EnemyType enemyType, int formationIndex)
{
	m_QueuedEnemies.push_back(std::pair(enemyType, formationIndex));
}

void EnemyManager::SpawnEnemy(EnemyType enemyType, int formationIndex)
{
	switch (enemyType)
	{
	case EnemyType::Bee:
		SpawnBee(enemyType, formationIndex);
		break;
	case EnemyType::Butterfly:
		SpawnButterfly(enemyType, formationIndex);
		break;
	case EnemyType::Boss:
		SpawnBoss(enemyType, formationIndex);
		break;
	}
}

void EnemyManager::SpawnBee(EnemyType enemyType, int formationIndex)
{
	const int beeWidth = 35;
	const int beeHeight = 41;
	//
	auto beeEnemy = std::make_shared<GameObject>("Bee");
	beeEnemy->AddComponent(new TransformComponent(glm::vec2(350 + 50, -10), glm::vec2(beeWidth, beeHeight)));
	beeEnemy->AddComponent(new Texture2DComponent("Bee.png", 1, false));
	beeEnemy->AddComponent(new EnemyStateManager(enemyType, formationIndex));
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(beeEnemy);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(beeEnemy);
}

void EnemyManager::SpawnButterfly(EnemyType enemyType, int formationIndex)
{
	const int beeWidth = 35;
	const int beeHeight = 39;
	//
	auto beeEnemy = std::make_shared<GameObject>("Butterfly");
	beeEnemy->AddComponent(new TransformComponent(glm::vec2(350 - 50, -10), glm::vec2(beeWidth, beeHeight)));
	beeEnemy->AddComponent(new Texture2DComponent("Butterfly.png", 1, false));
	beeEnemy->AddComponent(new EnemyStateManager(enemyType, formationIndex));
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(beeEnemy);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(beeEnemy);
}

void EnemyManager::SpawnBoss(EnemyType, int)
{
}

void EnemyManager::ClearEnemies()
{
	m_Index = 0;
	m_SpawnTime = 0.0f;
	m_QueuedEnemies.clear();
}