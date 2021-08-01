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
	void SetFormationIndex(int formationRowIndex, int formationPosIndex) { m_FormationPosIndex = formationPosIndex; m_FormationRowIndex = formationRowIndex; }

	bool GetSwitchState() const { return m_SwitchState; }
protected:
	bool m_SwitchState = false;
	//formation vars
	int m_FormationPosIndex = 0;
	int m_FormationRowIndex = 0;
	//shoot vars
	bool m_CanShoot = false;
	float m_ShootTime = 0.0f;
	int m_ShootTimer = 0;
	//
	BezierPathManager* m_pBezierPathManager = nullptr;
};
