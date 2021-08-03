#include "MiniginPCH.h"
#include "EnemySpawnState.h"
#include "EnemyStateManager.h"
#include "EnemyFormationState.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "FormationManager.h"
#include "EnemyManager.h"
#include "EnemyWeaponComponent.h"

EnemySpawnState::EnemySpawnState()
{
	m_SwitchState = false;
	m_pBezierPathManager = new BezierPathManager();
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
	m_ShootTimer = 2;
}

EnemySpawnState::~EnemySpawnState()
{
	delete m_pBezierPathManager;
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

void EnemySpawnState::Enter(EnemyStateManager&)
{
}

void EnemySpawnState::CreatePaths(EnemyStateManager& enemyStateMngr)
{
	auto startPos = enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto endPos = FormationManager::GetInstance().GetSpecificPos(m_FormationRowIndex, m_FormationPosIndex, enemyStateMngr.GetEnemyType());
	//
	BezierPath* path = new BezierPath();

	if (enemyStateMngr.GetEnemyType() == EnemyType::Bee)
	{
		path->AddCurve({ glm::vec2(startPos.x,startPos.y), glm::vec2(385,850 - 615), glm::vec2(90,850 - 690),glm::vec2(75,850 - 400) }, 30);
		path->AddCurve({ glm::vec2(75,850 - 400), glm::vec2(70,850 - 230) ,glm::vec2(600,750), glm::vec2(endPos.x,endPos.y + 50) }, 30);
		path->AddCurve({ glm::vec2(endPos.x,endPos.y + 50), glm::vec2(endPos.x,endPos.y + 50), endPos,glm::vec2(endPos.x + 30,endPos.y) }, 10);
	}
	else if (enemyStateMngr.GetEnemyType() == EnemyType::Butterfly)
	{
		path->AddCurve({ glm::vec2(startPos.x,startPos.y), glm::vec2(315,850 - 615), glm::vec2(610,850 - 690),glm::vec2(625,850 - 400) }, 30);
		path->AddCurve({ glm::vec2(625,850 - 400), glm::vec2(620,850 - 230) ,glm::vec2(100,750), glm::vec2(endPos.x,endPos.y + 50) }, 30);
		path->AddCurve({ glm::vec2(endPos.x,endPos.y + 50), glm::vec2(endPos.x,endPos.y + 50), endPos,glm::vec2(endPos.x - 30,endPos.y) }, 10);
	}

	m_pBezierPathManager->CreatePaths(path);
	delete path;
}