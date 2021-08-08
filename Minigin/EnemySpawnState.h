#pragma once
#include "EnemyState.h"
#include "EnemyManager.h"

class EnemySpawnState final : public EnemyState
{
public:
	EnemySpawnState();
	//
	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
private:
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
};
