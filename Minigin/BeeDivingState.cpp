#include "MiniginPCH.h"
#include "BeeDivingState.h"
#include "TransformComponent.h"
#include "BeeFormationState.h"
#include "FormationManager.h"

BeeDivingState::BeeDivingState()
{
	m_SwitchState = { false };
	m_pBezierPathManager = new BezierPathManager();
}

BeeDivingState::~BeeDivingState()
{
	delete m_pBezierPathManager;
}

void BeeDivingState::Update(BeeStateManager& bee)
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

BeeState* BeeDivingState::StateSwitch()
{
	return new BeeFormationState();
}

void BeeDivingState::Enter(BeeStateManager&)
{
}

void BeeDivingState::CreatePaths(BeeStateManager& bee)
{
	auto startPos = bee.GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto curvePoint = glm::vec2(startPos.x, 950);
	//
	BezierPath* path = new BezierPath();
	if (m_FormationIndex < 5)
	{
		path->AddCurve({ startPos, glm::vec2(startPos.x - 320,startPos.y - 110), glm::vec2(curvePoint.x + 620 ,curvePoint.y),curvePoint }, 30);
		path->AddCurve({ curvePoint, glm::vec2(curvePoint.x - 120,curvePoint.y - 180) ,glm::vec2(startPos.x - 70,startPos.y + 130),startPos }, 30);
	}
	else
	{
		path->AddCurve({ startPos, glm::vec2(startPos.x + 320,startPos.y - 110), glm::vec2(curvePoint.x - 620 ,curvePoint.y),curvePoint }, 30);
		path->AddCurve({ curvePoint, glm::vec2(curvePoint.x + 120,curvePoint.y - 180) ,glm::vec2(startPos.x + 70,startPos.y + 130),startPos }, 30);
	}

	m_pBezierPathManager->CreatePaths(path);
	delete path;
}