#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyStateManager.h"
#include <algorithm>

void FormationManager::Init()
{
	const int posY = 300;

	for (size_t i = 0; i < 2; i++)
	{
		std::vector<std::pair<glm::vec2, GameObject*>> tempPos;
		for (size_t j = 0; j < 10; j++)
		{
			tempPos.push_back(std::make_pair(glm::vec2{ 100 + j * 50 ,posY - i * 50 }, nullptr));
			if (j >= 9)
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
		for (size_t j = 0; j < m_BeePositions[i].size(); j++)
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