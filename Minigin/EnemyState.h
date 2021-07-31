#pragma once
#include "EnemyStateManager.h"
#include "BezierPathManager.h"

class EnemyState
{
public:
	EnemyState() = default;
	virtual ~EnemyState() = default;

	virtual void Update(EnemyStateManager& enemyStateMngr) = 0;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) = 0;
	virtual void Enter(EnemyStateManager& enemyStateMngr) = 0;
	virtual void CreatePaths(EnemyStateManager& enemyStateMngr) = 0;
	void SetFormationIndex(int formationIndex) { m_FormationIndex = formationIndex; }

	bool GetSwitchState() const { return m_SwitchState; }
protected:
	bool m_SwitchState = false;
	int m_FormationIndex = 0;
	//
	BezierPathManager* m_pBezierPathManager = nullptr;
};
