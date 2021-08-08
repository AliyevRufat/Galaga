#pragma once
#include "EnemyState.h"
#include "EnemyManager.h"

class EnemyGoToBeamState final : public EnemyState
{
public:
	EnemyGoToBeamState(EnemyStateManager& enemyStateMngr);
	~EnemyGoToBeamState();

	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
};
