#include "MiniginPCH.h"
#include "BulletMovementComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "EngineTime.h"

BulletMovementComponent::BulletMovementComponent(const glm::vec2& target)
	:m_Target{ target }
	, m_TargetOffset{ 2,2 }
	, m_LerpT{ 0.0f }
{
}
glm::vec2 startPos;
void BulletMovementComponent::Init()
{
	if (m_Target != glm::vec2(0, 0))
	{
		auto currentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		startPos = currentPos;
		if (m_Target.x - currentPos.x < 0) //to extend the vector in the correct side
		{
			//m_TargetOffset.x *= -1;
		}
	}
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
		auto BA = m_Target - startPos;
		auto furtherTarget = glm::vec2(BA.x * 2, BA.y * 2);
		auto newTarget = glm::vec2(furtherTarget.x + startPos.x, furtherTarget.y + startPos.y);

		speed = 30;
		float newX = currentPos.x + m_LerpT * (newTarget.x - currentPos.x);
		float newY = currentPos.y + m_LerpT * (newTarget.y - currentPos.y);

		m_LerpT += EngineTime::GetInstance().GetDeltaTime() / (float)speed;
		//
		transformComponent->SetPosition(glm::vec2{ newX,newY });
	}
}