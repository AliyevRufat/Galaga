#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"
#include "BeeStateManager.h"
#include <algorithm>

void FormationManager::Init()
{
	const int posY = 300;

	for (size_t i = 0; i < 10; i++)
	{
		m_BeePositions.push_back(std::make_pair(glm::vec2{ 100 + i * 50 ,posY }, nullptr));
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

glm::vec2 FormationManager::SaveAvailablePosInFormation(GameObject* gameObject)
{
	for (int i = 0; i < m_BeePositions.size(); i++)
	{
		if (m_BeePositions[i].second == nullptr)
		{
			m_BeePositions[i].second = gameObject;
			return m_BeePositions[i].first;
		}
	}
	return glm::vec2{ 0,0 };
}

glm::vec2 FormationManager::GetSpecificPos(GameObject* gameObject)
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		if (m_BeePositions[i].second == gameObject)
		{
			return m_BeePositions[i].first;
		}
	}
	return glm::vec2{ 0,0 };
}

void FormationManager::ClearFormation()
{
	for (size_t i = 0; i < m_BeePositions.size(); i++)
	{
		m_BeePositions[i].second = nullptr;
	}
}