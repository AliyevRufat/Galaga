#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "EnemyFormationState.h"
#include "EnemyDivingState.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyWeaponComponent.h"
#include "EnemyGoToBeamState.h"
#include "TractorBeamComponent.h"

EnemyFormationState::EnemyFormationState()
	:m_TimerBeforeDiving{ rand() % 7 + 2 }
	, m_TimeBeforeDiving{ 0.0f }
{
	m_SwitchState = { false };
	//if can shoot
	const int randNr = rand() % 10 + 1;
	if (randNr == EnemyManager::GetInstance().GetEnemyChanceToShoot())
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
	//update pos in formation
	enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->SetPosition(FormationManager::GetInstance().GetSpecificPos(m_FormationRowIndex, m_FormationPosIndex, enemyStateMngr.GetEnemyType()));
	//
	if (!EnemyManager::GetInstance().GetAllEnemiesAreSpawned())
	{
		return;
	}
	//timer before getting out of the formation
	m_TimeBeforeDiving += EngineTime::GetInstance().GetDeltaTime();

	if (EnemyManager::GetInstance().CanDive(enemyStateMngr.GetEnemyType()) && m_TimeBeforeDiving >= m_TimerBeforeDiving)
	{
		m_TimeBeforeDiving -= m_TimeBeforeDiving;
		m_SwitchState = true;
	}
	//
	ShootBullet(enemyStateMngr);
}

EnemyState* EnemyFormationState::StateSwitch(EnemyStateManager& enemyStateMngr)
{
	if (enemyStateMngr.GetEnemyType() == EnemyType::Boss)
	{
		if (enemyStateMngr.GetGameObject()->GetComponent<TractorBeamComponent>()->GetIsPlayerCaught())//if boss has caught a player it can't go tractor beam again
		{
			return new EnemyDivingState(enemyStateMngr);
		}
		const int randNr = rand() % 2;
		//
		if (randNr == 0)
		{
			return new EnemyGoToBeamState(enemyStateMngr);
		}
		else
		{
			return new EnemyDivingState(enemyStateMngr);
		}
	}
	//
	return new EnemyDivingState(enemyStateMngr);
}

void EnemyFormationState::Enter(EnemyStateManager& enemyStateManager)
{
	CreatePaths(enemyStateManager);
}