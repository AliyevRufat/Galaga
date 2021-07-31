#pragma once
#include "EnemyState.h"

class GoToFormationState final : public EnemyState
{
public:
	GoToFormationState();
	~GoToFormationState();

	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
};
