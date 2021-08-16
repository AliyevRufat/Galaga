#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "EnemyFormationState.h"
#include "EnemyDivingState.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyWeaponComponent.h"
#include "EnemyGoToBeamState.h"
#include "TractorBeamComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "StageManager.h"
#include "VersusControllerComponent.h"

EnemyFormationState::EnemyFormationState()
	:m_TimerBeforeDiving{ rand() % 7 + 2 }
	, m_TimeBeforeDiving{ 0.0f }
	, m_pBossState{ nullptr }
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
	//
	ShootBullet(enemyStateMngr);
	//
	if (enemyStateMngr.GetEnemyType() == EnemyType::Boss && StageManager::GetInstance().GetCurrentGameMode() == StageManager::GameMode::Versus)
	{
		auto versusControlllerComp = enemyStateMngr.GetGameObject()->GetComponent<VersusControllerComponent>();
		if (versusControlllerComp)
		{
			if (versusControlllerComp->GetIsTractorBeamActivated())
			{
				m_pBossState = new EnemyGoToBeamState(enemyStateMngr);
				versusControlllerComp->SetIsTractorBeamActivated(false);
				m_SwitchState = true;
			}
			else if (versusControlllerComp->GetIsDivingActivated())
			{
				m_pBossState = new EnemyDivingState(enemyStateMngr);
				versusControlllerComp->SetIsDivingActivated(false);
				m_SwitchState = true;
			}
			else if (versusControlllerComp->GetIsShootingActivated())
			{
				m_CanShoot = true;
				m_ShootTimer = 1;
				versusControlllerComp->SetIsShootingActivated(false);
			}
		}
		return;
	}
	//timer before getting out of the formation
	m_TimeBeforeDiving += EngineTime::GetInstance().GetDeltaTime();

	if (m_TimeBeforeDiving >= m_TimerBeforeDiving)
	{
		m_TimeBeforeDiving -= m_TimeBeforeDiving;
		m_SwitchState = true;
	}
}

EnemyState* EnemyFormationState::StateSwitch(EnemyStateManager& enemyStateMngr)
{
	auto currGameMode = StageManager::GetInstance().GetCurrentGameMode();
	if ((currGameMode == StageManager::GameMode::Versus && enemyStateMngr.GetEnemyType() != EnemyType::Boss) || currGameMode == StageManager::GameMode::Coop || currGameMode == StageManager::GameMode::SinglePlayer)
	{
		if (!EnemyManager::GetInstance().CanDive(enemyStateMngr.GetEnemyType()))
		{
			return nullptr;
		}
	}
	//if player is dead don't dive
	if (StageManager::GetInstance().GetCurrentGameMode() == StageManager::GameMode::Coop)
	{
		auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);
		auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);
		if (player && !player->GetIsActive() && player2 && !player2->GetIsActive())
		{
			return nullptr;
		}
	}
	else
	{
		auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);
		if (player && !player->GetIsActive())
		{
			return nullptr;
		}
	}
	//
	if (enemyStateMngr.GetEnemyType() == EnemyType::Boss)
	{
		if (m_pBossState)
		{
			return m_pBossState;
		}
		else
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
	}
	//
	return new EnemyDivingState(enemyStateMngr);
}

void EnemyFormationState::Enter(EnemyStateManager& enemyStateManager)
{
	CreatePaths(enemyStateManager);
}