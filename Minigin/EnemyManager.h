#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "ComponentIncludes.h"
#include <vector>

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

	void QueueEnemy(EnemyType enemyType, int formationRowIndex, int formationIndex);

	void IncreaseDifficulty();
	void IncreaseAmountOfDivingEnemies(EnemyType enemyType);
	void DecreaseAmountOfDivingEnemies(EnemyType enemyType);

	int GetEnemyChanceToShoot() const;
	bool GetAllEnemiesAreSpawned() const;
	bool CanDive(EnemyType enemyType) const;

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
	//
	int m_Index = 0;
	float m_SpawnTime = 0.0f;
	const float m_SpawnTimer = 0.2f;
	//
	int m_AmountOfDivingBees = 0;
	int m_AmountOfDivingButterflies = 0;
	int m_AmountOfDivingBosses = 0;
	//
	int m_MaxAmountOfDivingEnemies = 1;
	//
	int m_EnemyChanceToShoot = 2;//20%
	bool m_AllEnemiesAreSpawned = false;
};
