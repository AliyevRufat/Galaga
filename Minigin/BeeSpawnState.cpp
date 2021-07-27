#include "MiniginPCH.h"
#include "BeeSpawnState.h"
#include "BeeFormationState.h"
#include "BezierPath.h"

BeeSpawnState::BeeSpawnState()
{
	m_SwitchState = false;
}

void BeeSpawnState::Update(BeeStateManager&)
{
}

BeeState* BeeSpawnState::StateSwitch()
{
	return new BeeFormationState();
}

void BeeSpawnState::Enter(BeeStateManager&)
{
}

void BeeSpawnState::CreatePaths()
{
	int currentPath = 0;
	BezierPath* path = new BezierPath();
	//
	path->AddCurve({ glm::vec2(500.0f,10.0f), glm::vec2(500.0f,0.0f), glm::vec2(500.0f,310.0f), glm::vec2(500.0f,300.0f) }, 1);

	m_Paths.push_back(std::vector<glm::vec2>());
	path->Sample(&m_Paths[currentPath]);
	delete path;
}