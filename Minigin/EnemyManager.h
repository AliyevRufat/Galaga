#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "ComponentIncludes.h"
#include "StageManager.h"
#include <vector>
#include <deque>

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

	void SpawnAllEnemies(StageManager::Stage stage);

	void IncreaseDifficulty();
	void IncreaseAmountOfDivingEnemies(EnemyType enemyType);
	void DecreaseAmountOfDivingEnemies(EnemyType enemyType);

	int GetEnemyChanceToShoot() const;
	bool GetAllEnemiesAreSpawned() const;
	bool GetAllEnemiesAreDead()const;
	std::shared_ptr<GameObject> GetAvailableBoss() const;
	bool CanDive(EnemyType enemyType) const;

	void DeleteAllEnemies();
	void DeleteSpecificEnemy(const std::shared_ptr<GameObject>& enemy);

	std::pair<BezierPath*, glm::vec2> GetSpawnPath(EnemyType enemyType, int formationPosIndex, const glm::vec2& endPos);

private:
	friend class dae::Singleton<EnemyManager>;
	EnemyManager() = default;
	//Methods
	void Wait();
	void QueueEnemy(EnemyType enemyType, int formationRowIndex, int formationIndex, bool secondQueue = false);
	void SpawnEnemy(EnemyType enemyType, int formationRowIndex, int formationIndex);
	//
	void SpawnBee(EnemyType enemyType, int formationRowIndex, int formationIndex);
	void SpawnButterfly(EnemyType enemyType, int formationRowIndex, int formationIndex);
	void SpawnBoss(EnemyType enemyType, int formationRowIndex, int formationIndex);
	//Datamembers
	std::deque<std::pair<EnemyType, std::pair<int, int>>> m_QueuedEnemies;
	std::deque<std::pair<EnemyType, std::pair<int, int>>> m_SecondQueuedEnemies;
	//
	std::vector<std::shared_ptr<GameObject>> m_SpEnemies;
	std::vector<std::shared_ptr<GameObject>> m_spBosses;
	//
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
	bool m_StageStarted = false;
	//
	int m_EnemyMovementSpeed = 50; // start with 500
	//
	const int m_WaitTimer = 1;
	float m_WaitTime = 0.0f;
	std::vector<int> m_WaitIndices;
};
