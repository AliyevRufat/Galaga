#include "MiniginPCH.h"
#include "EnemyFormationState.h"
#include "EnemyDivingState.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyWeaponComponent.h"

EnemyFormationState::EnemyFormationState()
	:m_TimerBeforeDiving{ rand() % 12 + 3 }
	, m_TimeBeforeDiving{ 0.0f }
{
	m_SwitchState = { false };
	//if can shoot
	const int randNr = rand() % 10 + 1;
	if (randNr == 1)// 20% chance that it can shoot
	{
		m_CanShoot = true;
	}
	else
	{
		m_CanShoot = false;
	}
	m_ShootTimer = m_TimerBeforeDiving - 2;
}

void EnemyFormationState::Update(EnemyStateManager& enemyStateMngr)
{
	//timer before getting out of the formation
	m_TimeBeforeDiving += EngineTime::GetInstance().GetDeltaTime();

	if (EnemyManager::GetInstance().CanDive() && m_TimeBeforeDiving >= m_TimerBeforeDiving)
	{
		m_TimeBeforeDiving -= m_TimeBeforeDiving;
		m_SwitchState = true;
	}
	//update pos in formation
	enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->SetPosition(FormationManager::GetInstance().GetSpecificPos(m_FormationRowIndex, m_FormationPosIndex, enemyStateMngr.GetEnemyType()));
	//shooting
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

EnemyState* EnemyFormationState::StateSwitch(EnemyStateManager&)
{
	return new EnemyDivingState();
}

void EnemyFormationState::Enter(EnemyStateManager&)
{
}

void EnemyFormationState::CreatePaths(EnemyStateManager&)
{
}