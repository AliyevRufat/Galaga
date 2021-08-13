#pragma once
#include "EnemyState.h"

class EnemyTractorBeamState final : public EnemyState
{
public:
	EnemyTractorBeamState(EnemyStateManager& enemyStateManager);
	//
	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
private:
	void CreatePaths(EnemyStateManager&) override {};
};
