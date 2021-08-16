#include "MiniginPCH.h"
#include "LerpComponent.h"
#include "TransformComponent.h"
#include "EngineTime.h"

LerpComponent::LerpComponent(const glm::vec2& endPos, int speed)
	: m_EndPos{ endPos }
	, m_LerpT{ 0.0f }
	, m_IsAtEndPos{ false }
	, m_Speed{ speed }
{
}

void LerpComponent::Update()
{
	if (m_IsAtEndPos)
	{
		return;
	}
	//
	auto currentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	//
	float newX = currentPos.x + m_LerpT * (m_EndPos.x - currentPos.x);
	float newY = currentPos.y + m_LerpT * (m_EndPos.y - currentPos.y);
	//
	m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(newX, newY));
	//
	m_LerpT += EngineTime::GetInstance().GetDeltaTime() / (float)m_Speed;
	//
	if (abs(currentPos.x - m_EndPos.x) <= 1 && abs(currentPos.y - m_EndPos.y) <= 1)
	{
		m_IsAtEndPos = true;
	}
}

bool LerpComponent::GetIsAtEndPos() const
{
	return m_IsAtEndPos;
}