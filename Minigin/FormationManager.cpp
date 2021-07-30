#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "BeeStateManager.h"
#include <algorithm>

void FormationManager::Init()
{
	const int posY = 300;

	std::vector<std::pair<glm::vec2, GameObject*>> tempPos;

	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			tempPos.push_back(std::make_pair(glm::vec2{ 100 + i * 50 ,posY - j * 50 }, nullptr));
			if (i >= 9)
			{
				m_BeePositions.push_back(tempPos);
			}
		}
	}
}

void FormationManager::Update()
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		for (size_t j = 0; j < m_BeePositions[i].size(); j++)
		{
			m_BeePositions[i][j].first.x = m_BeePositions[i][j].first.x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
		}
	}

	if (m_TimeBeforeMovingToOtherSide >= m_TimerBeforeMovingToOtherSide)
	{
		m_TimeBeforeMovingToOtherSide -= m_TimeBeforeMovingToOtherSide;
		m_Speed *= -1;
	}
	m_TimeBeforeMovingToOtherSide += EngineTime::GetInstance().GetDeltaTime();
}

glm::vec2 FormationManager::SaveAvailablePosInFormation(GameObject* gameObject, int formationIndex)
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		if (m_BeePositions[i][formationIndex].second == nullptr)
		{
			m_BeePositions[i][formationIndex].second = gameObject;
			return m_BeePositions[i][formationIndex].first;
		}
	}
	return glm::vec2{ 0,0 };
}

glm::vec2 FormationManager::GetSpecificPos(GameObject* gameObject)
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		for (size_t j = 0; j < m_BeePositions.size(); j++)
		{
			if (m_BeePositions[i][j].second == gameObject)
			{
				return m_BeePositions[i][j].first;
			}
		}
	}
	return glm::vec2{ 0,0 };
}

void FormationManager::ClearFormation()
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		for (size_t j = 0; j < m_BeePositions[i].size(); j++)
		{
			m_BeePositions[i][j].second = nullptr;
		}
	}
}