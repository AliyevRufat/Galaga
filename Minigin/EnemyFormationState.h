#pragma once
#include "EnemyState.h"

class EnemyFormationState final : public EnemyState
{
public:
	EnemyFormationState();

	void Update(EnemyStateManager& enemyStateMngr) override;
	void Enter(EnemyStateManager& enemyStateMngr) override;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) override;
	void CreatePaths(EnemyStateManager& enemyStateMngr) override;
private:

	const int m_TimerBeforeDiving;
	float m_TimeBeforeDiving;
};
