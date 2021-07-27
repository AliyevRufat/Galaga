#include "MiniginPCH.h"
#include "FormationManager.h"
#include "TransformComponent.h"

void FormationManager::Update()
{
	for (size_t i = 0; i < m_pBees.size(); i++)
	{
		auto transformComp = m_pBees[i]->GetComponent<TransformComponent>();
		const int posY = 100;
		int posX = 350; //TODO : change to the actual window height
		int offset = 20;
		if (i % 2)
		{
			//odd
			offset *= -1;
		}
		posX += offset;
		//extra movement

		transformComp->SetPosition(glm::vec2{ posX + m_Speed * EngineTime::GetInstance().GetDeltaTime(),posY });
	}

	if (m_TimeBeforeMovingToOtherSide >= m_TimerBeforeMovingToOtherSide)
	{
		m_TimeBeforeMovingToOtherSide -= m_TimeBeforeMovingToOtherSide;
		m_Speed *= -1;
	}
	m_TimeBeforeMovingToOtherSide += EngineTime::GetInstance().GetDeltaTime();
}

void FormationManager::AddBeeToFormation(const std::shared_ptr<GameObject>& bee)
{
	m_pBees.push_back(bee);
}