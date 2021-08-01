#include "MiniginPCH.h"
#include "BulletMovementComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "EngineTime.h"

BulletMovementComponent::BulletMovementComponent(const glm::vec2& target)
	:m_Target{ target }
	, m_LerpT{ 0.0f }
{
}

void BulletMovementComponent::Update()
{
	auto transformComponent = m_pGameObject->GetComponent<TransformComponent>();
	auto currentPos = transformComponent->GetTransform().GetPosition();
	int speed = 1500;
	//
	if (m_Target == glm::vec2{ 0,0 })
	{
		transformComponent->SetPosition(glm::vec2{ currentPos.x, currentPos.y - speed * EngineTime::GetInstance().GetDeltaTime() });
	}
	else
	{
		speed = 30;
		const int targetOffset = 2;
		float newX = currentPos.x + m_LerpT * (m_Target.x * targetOffset - currentPos.x);
		float newY = currentPos.y + m_LerpT * (m_Target.y * targetOffset - currentPos.y);
		m_LerpT += EngineTime::GetInstance().GetDeltaTime() / (float)speed;
		//
		transformComponent->SetPosition(glm::vec2{ newX,newY });
	}
}