#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyStateManager.h"
#include <algorithm>

void FormationManager::Init()
{
	const int posYBees = 300;
	const int posYButterflies = 200;

	for (size_t i = 0; i < 2; i++)
	{
		std::vector<glm::vec2> tempPos;
		for (size_t j = 0; j < 10; j++)
		{
			tempPos.push_back(glm::vec2{ 100 + j * 50 ,posYBees - i * 50 });
			if (j >= 9)
			{
				m_BeePositions.push_back(tempPos);
			}
		}
	}
	//
	for (size_t i = 0; i < 2; i++)
	{
		std::vector<glm::vec2> tempPos;
		for (size_t j = 0; j < 10; j++)
		{
			tempPos.push_back(glm::vec2{ 100 + j * 50 ,posYButterflies - i * 50 });
			if (j >= 9)
			{
				m_ButterflyPositions.push_back(tempPos);
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
			m_BeePositions[i][j].x = m_BeePositions[i][j].x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
		}
	}
	//
	for (size_t i = 0; i < m_ButterflyPositions.size(); i++)
	{
		for (size_t j = 0; j < m_ButterflyPositions[i].size(); j++)
		{
			m_ButterflyPositions[i][j].x = m_ButterflyPositions[i][j].x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
		}
	}

	if (m_TimeBeforeMovingToOtherSide >= m_TimerBeforeMovingToOtherSide)
	{
		m_TimeBeforeMovingToOtherSide -= m_TimeBeforeMovingToOtherSide;
		m_Speed *= -1;
	}
	m_TimeBeforeMovingToOtherSide += EngineTime::GetInstance().GetDeltaTime();
}

glm::vec2 FormationManager::GetSpecificPos(int rowIndex, int posIndex, EnemyType enemyType)
{
	if (enemyType == EnemyType::Bee)
	{
		return m_BeePositions[rowIndex][posIndex];
	}
	else if (enemyType == EnemyType::Butterfly)
	{
		return m_ButterflyPositions[rowIndex][posIndex];
	}
	//
	return glm::vec2{ 0,0 };
}