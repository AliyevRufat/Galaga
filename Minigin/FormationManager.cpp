#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyStateManager.h"
#include <algorithm>

void FormationManager::Init()
{
	const int posYBees = 300;
	const int posYButterflies = 200;
	const int posYBoss = 80;

	//make formation positions for the bees and the butterflies
	for (size_t i = 0; i < 2; i++)
	{
		std::vector<glm::vec2> tempPosBees;
		std::vector<glm::vec2> tempPosButterflies;
		for (size_t j = 0; j < 10; j++)
		{
			tempPosBees.push_back(glm::vec2{ 100 + j * 50 ,posYBees - i * 50 });
			tempPosButterflies.push_back(glm::vec2{ 100 + j * 50 ,posYButterflies - i * 50 });
			if (j >= 9)
			{
				m_BeePositions.push_back(tempPosBees);
				m_ButterflyPositions.push_back(tempPosButterflies);
			}
		}
	}
	//make formation positions for the bosses
	for (size_t i = 0; i < 4; i++)
	{
		m_BossPositions.push_back(glm::vec2{ m_BeePositions[0][3].x + i * 50 ,posYBoss });
	}
}

void FormationManager::Update()
{
	{ //formation left right movement
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
		//
		for (size_t i = 0; i < m_BossPositions.size(); i++)
		{
			m_BossPositions[i].x = m_BossPositions[i].x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
		}
	}
	//timer and the multiplier for moving left and right
	if (m_TimeBeforeMovingToOtherSide >= m_TimerBeforeMovingToOtherSide)
	{
		m_TimeBeforeMovingToOtherSide -= m_TimeBeforeMovingToOtherSide;
		m_Speed *= -1;
	}
	m_TimeBeforeMovingToOtherSide += EngineTime::GetInstance().GetDeltaTime();
}

glm::vec2 FormationManager::GetSpecificPos(int rowIndex, int posIndex, EnemyType enemyType) const
{
	if (enemyType == EnemyType::Bee)
	{
		return m_BeePositions[rowIndex][posIndex];
	}
	else if (enemyType == EnemyType::Butterfly)
	{
		return m_ButterflyPositions[rowIndex][posIndex];
	}
	else
	{
		return m_BossPositions[posIndex];
	}
}