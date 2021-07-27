#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "BeeStateManager.h"

void FormationManager::Init()
{
	const int posY = 300;

	for (size_t i = 0; i < 10; i++)
	{
		m_BeePositions.push_back(std::make_pair(glm::vec2{ 100 + i * 50 ,posY }, false));
	}
}

void FormationManager::Update()
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		m_BeePositions[i].first.x = m_BeePositions[i].first.x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
	}

	if (m_TimeBeforeMovingToOtherSide >= m_TimerBeforeMovingToOtherSide)
	{
		m_TimeBeforeMovingToOtherSide -= m_TimeBeforeMovingToOtherSide;
		m_Speed *= -1;
	}
	m_TimeBeforeMovingToOtherSide += EngineTime::GetInstance().GetDeltaTime();
}

int FormationManager::GetAvailablePosInFormation()
{
	for (int i = 0; i < m_BeePositions.size(); i++)
	{
		if (!m_BeePositions[i].second)
		{
			m_BeePositions[i].second = true;
			return i;
		}
	}
	return 0;
}

glm::vec2 FormationManager::GetPosWithIndex(int index)
{
	return m_BeePositions[index].first;
}

void FormationManager::ClearFormation()
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		m_BeePositions[i].second = false;
	}
}