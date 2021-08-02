#pragma once
#include "EnemyState.h"
#include "EnemyManager.h"

class EnemyDivingState final : public EnemyState
{
public:
	EnemyDivingState();
	~EnemyDivingState();

	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
};
