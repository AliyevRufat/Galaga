#include "MiniginPCH.h"
#include "LerpComponent.h"
#include "TransformComponent.h"
#include "EngineTime.h"

LerpComponent::LerpComponent(const glm::vec2& endPos)
	: m_EndPos{ endPos }
	, m_LerpT{ 0.0f }
	, m_IsAtEndPos{ false }
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
	const int speed = 50;
	float newX = currentPos.x + m_LerpT * (m_EndPos.x - currentPos.x);
	float newY = currentPos.y + m_LerpT * (m_EndPos.y - currentPos.y);
	//
	m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(newX, newY));
	//
	m_LerpT += EngineTime::GetInstance().GetDeltaTime() / (float)speed;
	//
	if (abs(currentPos.x - m_EndPos.x) <= 0.1f && abs(currentPos.y - m_EndPos.y) <= 0.1f)
	{
		m_IsAtEndPos = true;
	}
}

bool LerpComponent::GetIsAtEndPos() const
{
	return m_IsAtEndPos;
}