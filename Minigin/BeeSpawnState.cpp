#include "MiniginPCH.h"
#include "BeeSpawnState.h"
#include "BeeFormationState.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "FormationManager.h"

BeeSpawnState::BeeSpawnState()
{
	m_SwitchState = false;
	m_pBezierPathManager = new BezierPathManager();
}

BeeSpawnState::~BeeSpawnState()
{
	delete m_pBezierPathManager;
}

void BeeSpawnState::Update(BeeStateManager& bee)
{
	auto transformComp = bee.GetGameObject()->GetComponent<TransformComponent>();
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

BeeState* BeeSpawnState::StateSwitch()
{
	return new BeeFormationState();
}

void BeeSpawnState::Enter(BeeStateManager&)
{
}

void BeeSpawnState::CreatePaths(BeeStateManager& bee)
{
	auto startPos = bee.GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto endPos = FormationManager::GetInstance().SaveAvailablePosInFormation(bee.GetGameObject(), m_FormationIndex);
	//
	BezierPath* path = new BezierPath();
	path->AddCurve({ glm::vec2(startPos.x,startPos.y), glm::vec2(385,850 - 615), glm::vec2(90,850 - 690),glm::vec2(75,850 - 400) }, 30);
	path->AddCurve({ glm::vec2(75,850 - 400), glm::vec2(70,850 - 230) ,glm::vec2(600,750), glm::vec2(endPos.x,endPos.y + 50) }, 30);
	path->AddCurve({ glm::vec2(endPos.x,endPos.y + 50), glm::vec2(endPos.x,endPos.y + 50), endPos,glm::vec2(endPos.x + 30,endPos.y) }, 10);

	m_pBezierPathManager->CreatePaths(path);
	delete path;
}