#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "ComponentIncludes.h"
#include "StageManager.h"
#include <vector>

class BezierPath;
class TransformComponent;

enum class EnemyType
{
	Bee,
	Butterfly,
	Boss
};

class EnemyManager final : public dae::Singleton<EnemyManager>
{
public:

	void Update();

	void QueueEnemy(EnemyType enemyType, int formationRowIndex, int formationIndex, bool secondQueue = false);

	void IncreaseDifficulty();
	void IncreaseAmountOfDivingEnemies(EnemyType enemyType);
	void DecreaseAmountOfDivingEnemies(EnemyType enemyType);

	int GetEnemyChanceToShoot() const;
	bool GetAllEnemiesAreSpawned() const;
	bool CanDive(EnemyType enemyType) const;
	void Wait();

	void DeleteAllEnemies();

	std::pair<BezierPath*, glm::vec2> GetSpawnPath(StageManager::Stage stage, EnemyType enemyType, int formationPosIndex, const glm::vec2& endPos);

	void ClearEnemies();
private:
	friend class dae::Singleton<EnemyManager>;
	EnemyManager() = default;
	//Methods
	void SpawnEnemy(EnemyType enemyType, int formationRowIndex, int formationIndex);
	//
	void SpawnBee(EnemyType enemyType, int formationRowIndex, int formationIndex);
	void SpawnButterfly(EnemyType enemyType, int formationRowIndex, int formationIndex);
	void SpawnBoss(EnemyType enemyType, int formationIndex);
	//Datamembers
	std::vector<std::pair<EnemyType, std::pair<int, int>>> m_QueuedEnemies;
	std::vector<std::pair<EnemyType, std::pair<int, int>>> m_SecondQueuedEnemies;
	//
	std::vector<std::shared_ptr<GameObject>> m_SpEnemies;
	//
	int m_IndexBees = 0;
	int m_IndexButterfliesAndBosses = 0;
	float m_SpawnTimeBee = 0.0f;
	float m_SpawnTimeButterfly = 0.0f;
	const float m_SpawnTimer = 0.05f;
	//
	int m_AmountOfDivingBees = 0;
	int m_AmountOfDivingButterflies = 0;
	int m_AmountOfDivingBosses = 0;
	int m_AmountOfSpawnedEnemies = 0;
	//
	int m_MaxAmountOfDivingEnemies = 1;
	//
	int m_EnemyChanceToShoot = 1;//10%
	bool m_AllEnemiesAreSpawned = false;
	//
	const int m_WaitTimer = 1;
	float m_WaitTime = 0.0f;
	std::vector<int> m_WaitIndices;
};
