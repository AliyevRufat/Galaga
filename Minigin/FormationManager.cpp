#include "MiniginPCH.h"
#include "FormationManager.h"
#include "../AliEngine/TransformComponent.h"
#include "EnemyStateManager.h"
#include <algorithm>
#include <fstream>

void FormationManager::ParsePosOfEnemy(const std::string& oneLine, int level, char firstLetter, char secondLetter, int row)
{
	std::string posString;
	const int textOffset = 5;

	if (oneLine[0] == std::to_string(level)[0])//level
	{
		if (oneLine[1] == firstLetter && oneLine[2] == secondLetter) // reading pos of correct enemy
		{
			if (oneLine[3] == std::to_string(row)[0])//row
			{
				if (secondLetter == 'E')
				{
					if (m_BeePositions[level - 1].size() < size_t(row))
					{
						m_BeePositions[level - 1].push_back(std::vector<glm::vec2>());
					}
				}
				else if (secondLetter == 'U')
				{
					if (m_ButterflyPositions[level - 1].size() < size_t(row))
					{
						m_ButterflyPositions[level - 1].push_back(std::vector<glm::vec2>());
					}
				}
				else if (secondLetter == 'O')
				{
					if (m_BossPositions[level - 1].size() < size_t(row))
					{
						m_BossPositions[level - 1].push_back(std::vector<glm::vec2>());
					}
				}

				size_t delimeterIndex = oneLine.find_first_of(',');
				glm::vec2 beePos;
				posString.clear();
				for (size_t i = textOffset; i < oneLine.size(); i++)
				{
					if (delimeterIndex == i)
					{
						break;
					}
					posString += oneLine[i];
				}
				beePos.x = std::stof(posString);
				posString.clear();
				for (size_t i = delimeterIndex + 1; i < oneLine.size(); i++)
				{
					posString += oneLine[i];
				}
				beePos.y = std::stof(posString);
				//
				if (secondLetter == 'E')
				{
					m_BeePositions[level - 1][row - 1].push_back(beePos);
				}
				else if (secondLetter == 'U')
				{
					m_ButterflyPositions[level - 1][row - 1].push_back(beePos);
				}
				else if (secondLetter == 'O')
				{
					m_BossPositions[level - 1][row - 1].push_back(beePos);
				}
			}
		}
	}
}

void FormationManager::ReadFormationFromFile()
{
	m_BeePositions.clear();
	m_ButterflyPositions.clear();
	m_BossPositions.clear();
	//
	for (size_t i = 0; i < 3; i++)
	{
		m_BeePositions.push_back(std::vector<std::vector<glm::vec2>>());
		m_ButterflyPositions.push_back(std::vector<std::vector<glm::vec2>>());
		m_BossPositions.push_back(std::vector<std::vector<glm::vec2>>());
	}

	std::ifstream input;
	std::string oneLine;

	input.open("../Data/FormationCoordinates.txt", std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		int nr = 0;
		while (std::getline(input, oneLine))
		{
			if (nr == 82)
			{
				const int sdf = 0;
			}
			//level1
			ParsePosOfEnemy(oneLine, 1, 'B', 'E', 1);
			ParsePosOfEnemy(oneLine, 1, 'B', 'E', 2);
			ParsePosOfEnemy(oneLine, 1, 'B', 'U', 1);
			ParsePosOfEnemy(oneLine, 1, 'B', 'U', 2);
			ParsePosOfEnemy(oneLine, 1, 'B', 'O', 1);
			//level2
			ParsePosOfEnemy(oneLine, 2, 'B', 'E', 1);
			ParsePosOfEnemy(oneLine, 2, 'B', 'E', 2);
			ParsePosOfEnemy(oneLine, 2, 'B', 'E', 3);
			ParsePosOfEnemy(oneLine, 2, 'B', 'U', 1);
			ParsePosOfEnemy(oneLine, 2, 'B', 'U', 2);
			ParsePosOfEnemy(oneLine, 2, 'B', 'O', 1);
			ParsePosOfEnemy(oneLine, 2, 'B', 'O', 2);
			//level3
			ParsePosOfEnemy(oneLine, 3, 'B', 'E', 1);
			ParsePosOfEnemy(oneLine, 3, 'B', 'E', 2);
			ParsePosOfEnemy(oneLine, 3, 'B', 'E', 3);
			ParsePosOfEnemy(oneLine, 3, 'B', 'E', 4);
			ParsePosOfEnemy(oneLine, 3, 'B', 'E', 5);
			ParsePosOfEnemy(oneLine, 3, 'B', 'U', 1);
			ParsePosOfEnemy(oneLine, 3, 'B', 'U', 2);
			ParsePosOfEnemy(oneLine, 3, 'B', 'O', 1);
			ParsePosOfEnemy(oneLine, 3, 'B', 'O', 2);
			ParsePosOfEnemy(oneLine, 3, 'B', 'O', 3);
			++nr;
		}
		input.close();
	}
}

void FormationManager::InitFormation()
{
	ReadFormationFromFile();
	m_IsInitialized = true;
}

void FormationManager::Update()
{
	if (!m_IsInitialized)
	{
		return;
	}
	int levelInt = int(StageManager::GetInstance().GetCurrentStage());
	{ //formation left right movement
		for (size_t i = 0; i < m_BeePositions[levelInt].size(); i++)
		{
			for (size_t j = 0; j < m_BeePositions[levelInt][i].size(); j++)
			{
				m_BeePositions[levelInt][i][j].x = m_BeePositions[levelInt][i][j].x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
			}
		}
		//
		for (size_t i = 0; i < m_ButterflyPositions[levelInt].size(); i++)
		{
			for (size_t j = 0; j < m_ButterflyPositions[levelInt][i].size(); j++)
			{
				m_ButterflyPositions[levelInt][i][j].x = m_ButterflyPositions[levelInt][i][j].x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
			}
		}
		//
		for (size_t i = 0; i < m_BossPositions[levelInt].size(); i++)
		{
			for (size_t j = 0; j < m_BossPositions[levelInt][i].size(); j++)
			{
				m_BossPositions[levelInt][i][j].x = m_BossPositions[levelInt][i][j].x + m_Speed * EngineTime::GetInstance().GetDeltaTime();
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
	int levelInt = int(StageManager::GetInstance().GetCurrentStage());
	//
	if (enemyType == EnemyType::Bee)
	{
		return m_BeePositions[levelInt][rowIndex][posIndex];
	}
	else if (enemyType == EnemyType::Butterfly)
	{
		return m_ButterflyPositions[levelInt][rowIndex][posIndex];
	}
	else
	{
		return m_BossPositions[levelInt][rowIndex][posIndex];
	}
}