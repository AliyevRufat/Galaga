#pragma once
#include "EnemyState.h"

class EnemyGoToFormationState final : public EnemyState
{
public:
	EnemyGoToFormationState(EnemyStateManager& enemyStateMngr);
	//
	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
private:
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
};
