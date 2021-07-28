#include "MiniginPCH.h"
#include "BeeSpawnState.h"
#include "BeeFormationState.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "FormationManager.h"

glm::vec2 Normalize(const glm::vec2& vector)
{
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	if (length != 0)
		return glm::vec2(vector.x / length, vector.y / length);
	else
		return vector;
}
BeeSpawnState::BeeSpawnState(int path)
{
	m_SwitchState = false;

	m_CurrentPath = path;
}

void BeeSpawnState::Update(BeeStateManager& bee)
{
	auto pos = bee.GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	auto d = m_Paths[m_CurrentPath][m_CurrentWayPoint] - pos;

	float magnitudeSqr = sqrt(d.x * d.x + d.y * d.y);

	if (magnitudeSqr < EPSILON)
	{
		++m_CurrentWayPoint;
	}

	if (m_CurrentWayPoint < m_Paths[m_CurrentPath].size())
	{
		const float speed = 300.0f;
		glm::vec2 distance = m_Paths[m_CurrentPath][m_CurrentWayPoint] - pos;
		bee.GetGameObject()->GetComponent<TransformComponent>()->SetPosition(pos + Normalize(distance) * EngineTime::GetInstance().GetDeltaTime() * speed);
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
	auto endPos = FormationManager::GetInstance().SaveAvailablePosInFormation(bee.GetGameObject());
	//
	int currentPath = 0;
	BezierPath* path = new BezierPath();
	//
	//path->AddCurve({ glm::vec2(startPos.x,startPos.y), glm::vec2(startPos.x,startPos.y - 10.0f), glm::vec2(startPos.x,30.0f), glm::vec2(startPos.x,20.0f) }, 20);
	//path->AddCurve({ glm::vec2(startPos.x,20.0f), glm::vec2(startPos.x,100.0f), glm::vec2(75.0f,325.0f), glm::vec2(75.0f,425.0f) }, 20);
	//path->AddCurve({ glm::vec2(75.0f,425.0f), glm::vec2(75.0f,650.0f), endPos, glm::vec2(endPos.x,endPos.y + 100) }, 20);

	path->AddCurve({ glm::vec2(startPos.x,startPos.y), glm::vec2(385,850 - 615), glm::vec2(90,850 - 690),glm::vec2(75,850 - 400) }, 30);
	path->AddCurve({ glm::vec2(75,400), glm::vec2(70,850 - 230) ,glm::vec2(600,750), glm::vec2(endPos.x,endPos.y + 50) }, 30);
	path->AddCurve({ glm::vec2(endPos.x,endPos.y + 50), glm::vec2(endPos.x,endPos.y + 50) ,endPos, endPos }, 30);

	m_Paths.push_back(std::vector<glm::vec2>());
	path->Sample(&m_Paths[currentPath]);
	delete path;
}