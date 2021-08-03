#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "EnemyFormationState.h"
#include "EnemyDivingState.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyWeaponComponent.h"

EnemyFormationState::EnemyFormationState()
	:m_TimerBeforeDiving{ rand() % 7 + 2 }
	, m_TimeBeforeDiving{ 0.0f }
{
	m_SwitchState = { false };
	//if can shoot
	const int randNr = rand() % 5 + 1;
	if (randNr == 1)// 20% chance that it can shoot
	{
		m_CanShoot = true;
	}
	else
	{
		m_CanShoot = false;
	}
	m_ShootTimer = float(m_TimerBeforeDiving) - 2.0f;
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
	//
	ShootBullet(enemyStateMngr);
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