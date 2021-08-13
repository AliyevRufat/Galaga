#include "MiniginPCH.h"
#include "EnemyGoToBeamState.h"
#include "TransformComponent.h"
#include "EnemyFormationState.h"
#include "FormationManager.h"
#include "SceneManager.h"
#include "EnemyGoToFormationState.h"
#include "EnemyManager.h"
#include "EnemyWeaponComponent.h"
#include "EnemyTractorBeamState.h"

EnemyGoToBeamState::EnemyGoToBeamState(EnemyStateManager& enemyStateMngr)
{
	m_SwitchState = false;
	m_pBezierPathManager = new BezierPathManager(enemyStateMngr.GetEnemySpeed());
	EnemyManager::GetInstance().IncreaseAmountOfDivingEnemies(enemyStateMngr.GetEnemyType());
}

void EnemyGoToBeamState::Update(EnemyStateManager& enemyStateMngr)
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
}

EnemyState* EnemyGoToBeamState::StateSwitch(EnemyStateManager& enemyStateMngr)
{
	return new EnemyTractorBeamState(enemyStateMngr);
}

void EnemyGoToBeamState::Enter(EnemyStateManager& enemyStateManager)
{
	CreatePaths(enemyStateManager);
}

void EnemyGoToBeamState::CreatePaths(EnemyStateManager& enemyStateMngr)
{
	auto screenDimensions = dae::SceneManager::GetInstance().GetScreenDimensions();
	auto startPos = enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	//
	const int offset = 100;
	const int randTargetPoint = rand() % ((int)screenDimensions.x - offset * 2) + offset;
	const int endPosY = 550;

	//
	BezierPath* path = new BezierPath();

	path->AddCurve({ startPos, glm::vec2(startPos.x - 100.,startPos.y - 100), glm::vec2(startPos.x - 170,startPos.y + 80), glm::vec2(startPos.x + 80,startPos.y + 70) }, 30);
	path->AddCurve({ glm::vec2(startPos.x + 80,startPos.y + 70),glm::vec2(startPos.x + 320,startPos.y + 70), glm::vec2(startPos.x - 100,startPos.y - 350), glm::vec2(randTargetPoint,endPosY) }, 30);

	m_pBezierPathManager->CreatePaths(path);
	delete path;
}