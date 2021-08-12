#include "MiniginPCH.h"
#include "EnemySpawnState.h"
#include "EnemyStateManager.h"
#include "EnemyFormationState.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "FormationManager.h"
#include "StageManager.h"
#include "EnemyManager.h"
#include "EnemyWeaponComponent.h"

EnemySpawnState::EnemySpawnState()
{
	m_SwitchState = false;
	m_pBezierPathManager = new BezierPathManager(1200);
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
	m_ShootTimer = 2;
}

void EnemySpawnState::Update(EnemyStateManager& enemyStateMngr)
{
	auto transformComp = enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>();
	glm::vec2 newPos = m_pBezierPathManager->CalculatePath(transformComp->GetTransform().GetPosition());

	if (newPos != glm::vec2{ 0,0 })
	{
		transformComp->SetPosition(newPos);
	}
	else
	{
		m_SwitchState = true;
	}
	//
	ShootBullet(enemyStateMngr);
}

EnemyState* EnemySpawnState::StateSwitch(EnemyStateManager&)
{
	return new EnemyFormationState();
}

void EnemySpawnState::Enter(EnemyStateManager& enemyStateManager)
{
	CreatePaths(enemyStateManager);
}

void EnemySpawnState::CreatePaths(EnemyStateManager& enemyStateMngr)
{
	auto endPos = FormationManager::GetInstance().GetSpecificPos(m_FormationRowIndex, m_FormationPosIndex, enemyStateMngr.GetEnemyType());
	//
	std::pair<BezierPath*, glm::vec2> pathAndStartPos;

	pathAndStartPos = EnemyManager::GetInstance().GetSpawnPath(enemyStateMngr.GetEnemyType(), m_FormationPosIndex, endPos);
	enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->SetPosition(pathAndStartPos.second);
	//
	m_pBezierPathManager->CreatePaths(pathAndStartPos.first);
	delete pathAndStartPos.first;
}