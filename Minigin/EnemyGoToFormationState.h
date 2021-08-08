#pragma once
#include "EnemyState.h"

class EnemyGoToFormationState final : public EnemyState
{
public:
	EnemyGoToFormationState();
	~EnemyGoToFormationState();

	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
};
