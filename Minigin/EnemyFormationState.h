#pragma once
#include "EnemyState.h"

class EnemyFormationState final : public EnemyState
{
public:
	EnemyFormationState();
	//
	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
private:
	void CreatePaths(EnemyStateManager&) override {};
	//
	const int m_TimerBeforeDiving;
	float m_TimeBeforeDiving;
	EnemyState* m_pBossState;
};
