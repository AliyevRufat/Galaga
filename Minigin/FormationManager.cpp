#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "EnemyStateManager.h"
#include <algorithm>

void FormationManager::InitFormation(StageManager::Stage stage)
{
	if (stage == StageManager::Stage::One)
	{
		const int posYBees = 300;
		const int posYButterflies = 200;
		const int posYBoss = 80;

		//make formation positions for the bees and the butterflies
		for (size_t i = 0; i < 2; i++)
		{
			std::vector<glm::vec2> tempPosBees;
			std::vector<glm::vec2> tempPosButterflies;
			//
			for (size_t j = 0; j < 8; j++)
			{
				tempPosButterflies.push_back(glm::vec2{ 150 + j * 50 ,posYButterflies - i * 50 });
				if (j >= 7)
				{
					m_ButterflyPositions.push_back(tempPosButterflies);
				}
			}
			for (size_t j = 0; j < 10; j++)
			{
				tempPosBees.push_back(glm::vec2{ 100 + j * 50 ,posYBees - i * 50 });
				if (j >= 9)
				{
					m_BeePositions.push_back(tempPosBees);
				}
			}
		}
		//make formation positions for the bosses
		m_BossPositions.push_back(std::vector<glm::vec2>());
		for (size_t i = 0; i < 4; i++)
		{
			m_BossPositions[0].push_back(glm::vec2{ m_BeePositions[0][3].x + i * 50 ,posYBoss });
		}
	}
	else if (stage == StageManager::Stage::Two)
	{
		const int posYBees = 150;
		const int posYButterfliesUp = 100;
		const int posYButterfliesDown = 300;
		const int posYBossUp = 80;
		const int posYBossDown = 300;

		//make formation positions for the bees and the butterflies

		for (size_t i = 0; i < 2; i++)
		{
			int posY = 0;
			if (i == 0)
			{
				posY = posYButterfliesUp;
			}
			else
			{
				posY = posYButterfliesDown;
			}
			std::vector<glm::vec2> tempPosButterflies;

			for (size_t j = 0; j < 4; j++)
			{
				tempPosButterflies.push_back(glm::vec2{ 250 + j * 50 ,posY });
				if (j >= 3)
				{
					m_ButterflyPositions.push_back(tempPosButterflies);
				}
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			std::vector<glm::vec2> tempPosBees;

			for (size_t j = 0; j < 10; j++)
			{
				tempPosBees.push_back(glm::vec2{ 100 + j * 50 ,posYBees + i * 50 });
				if (j >= 9)
				{
					m_BeePositions.push_back(tempPosBees);
				}
			}
		}
		//make formation positions for the bosses
		m_BossPositions.push_back(std::vector<glm::vec2>());
		m_BossPositions.push_back(std::vector<glm::vec2>());
		//
		m_BossPositions[0].push_back(glm::vec2{ m_ButterflyPositions[0][0].x - 50 ,posYBossUp });
		m_BossPositions[0].push_back(glm::vec2{ m_ButterflyPositions[0][3].x + 50 ,posYBossUp });
		m_BossPositions[1].push_back(glm::vec2{ m_ButterflyPositions[0][0].x - 50 ,posYBossDown });
		m_BossPositions[1].push_back(glm::vec2{ m_ButterflyPositions[0][3].x + 50 ,posYBossDown });
	}
	else
	{
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
			for (size_t j = 0; j < m_BossPositions[i].size(); j++)
			{
				m_BossPositions[i][j].x = m_BossPositions[i][j].x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
			}
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
		return m_BossPositions[rowIndex][posIndex];
	}
}