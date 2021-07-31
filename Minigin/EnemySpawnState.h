#pragma once
#include "EnemyState.h"

class EnemySpawnState final : public EnemyState
{
public:
	EnemySpawnState();
	~EnemySpawnState();

	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
};
