#include "MiniginPCH.h"
#include "GoToFormationState.h"
#include "EnemyFormationState.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "FormationManager.h"

GoToFormationState::GoToFormationState()
{
	m_SwitchState = false;
	m_pBezierPathManager = new BezierPathManager();
}

GoToFormationState::~GoToFormationState()
{
	delete m_pBezierPathManager;
}

void GoToFormationState::Update(EnemyStateManager& enemyStateMngr)
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

void GoToFormationState::Enter(EnemyStateManager&)
{
}

EnemyState* GoToFormationState::StateSwitch(EnemyStateManager&)
{
	return new EnemyFormationState();
}

void GoToFormationState::CreatePaths(EnemyStateManager& enemyStateMngr)
{
	auto startPos = enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto endPos = FormationManager::GetInstance().GetSpecificPos(m_FormationRowIndex, m_FormationPosIndex, enemyStateMngr.GetEnemyType());
	//
	BezierPath* path = new BezierPath();
	path->AddCurve({ startPos,startPos, endPos,endPos }, 1);

	m_pBezierPathManager->CreatePaths(path);
	delete path;
}