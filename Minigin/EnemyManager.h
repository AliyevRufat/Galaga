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
	void QueueEnemy(EnemyType enemyType, int formationIndex);
	void ClearEnemies();
private:
	friend class dae::Singleton<EnemyManager>;
	EnemyManager() = default;
	//Methods
	void SpawnEnemy(EnemyType enemyType, int formationIndex);
	//
	void SpawnBee(EnemyType enemyType, int formationIndex);
	void SpawnButterfly(EnemyType enemyType, int formationIndex);
	void SpawnBoss(EnemyType enemyType, int formationIndex);
	//Datamembers
	std::vector<std::pair<EnemyType, int>> m_QueuedEnemies;
	//
	int m_Index = 0;
	float m_SpawnTime = 0.0f;
	const float m_SpawnTimer = 0.2f;
};
