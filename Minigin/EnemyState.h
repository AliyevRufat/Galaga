#pragma once
#include "EnemyStateManager.h"
#include "BezierPathManager.h"
#include "EnemyWeaponComponent.h"

class EnemyState
{
public:
	EnemyState() = default;
	virtual ~EnemyState() { delete m_pBezierPathManager; };
	EnemyState(const EnemyState& other) = delete;
	EnemyState(EnemyState&& other) = delete;
	EnemyState& operator=(const EnemyState& other) = delete;
	EnemyState& operator=(EnemyState&& other) = delete;
	//
	virtual void Update(EnemyStateManager& enemyStateMngr) = 0;
	virtual EnemyState* StateSwitch(EnemyStateManager& enemyStateMngr) = 0;
	virtual void Enter(EnemyStateManager& enemyStateMngr) = 0;
	//
	void SetFormationIndex(int formationRowIndex, int formationPosIndex) { m_FormationPosIndex = formationPosIndex; m_FormationRowIndex = formationRowIndex; }
	bool GetSwitchState() const { return m_SwitchState; }
	void ShootBullet(EnemyStateManager& enemyStateMngr)
	{
		if (m_CanShoot)
		{
			m_ShootTime += EngineTime::GetInstance().GetDeltaTime();
			if (m_ShootTime >= m_ShootTimer)
			{
				m_CanShoot = false;
				m_ShootTime -= m_ShootTime;
				//shoot
				enemyStateMngr.GetGameObject()->GetComponent<EnemyWeaponComponent>()->Shoot();
			}
		}
	}

protected:
	virtual void CreatePaths(EnemyStateManager& enemyStateMngr) = 0;
	//
	bool m_SwitchState = false;
	//formation vars
	int m_FormationPosIndex = 0;
	int m_FormationRowIndex = 0;
	//shooting vars
	bool m_CanShoot = false;
	float m_ShootTime = 0.0f;
	float m_ShootTimer = 0.0f;
	//
	BezierPathManager* m_pBezierPathManager = nullptr;
};
