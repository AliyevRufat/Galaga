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
	const int speed = 1500;
	//
	if (m_Target == glm::vec2{ 0,0 })
	{
		transformComponent->SetPosition(glm::vec2{ currentPos.x, currentPos.y - speed * EngineTime::GetInstance().GetDeltaTime() });
	}
	else
	{
		//lerp between current pos and target // TODO : untested, probably not working coz you rewrite the start pos every time
		float newX = currentPos.x + m_LerpT * (m_Target.x - currentPos.x);
		float newY = currentPos.y + m_LerpT * (m_Target.y - currentPos.y);
		m_LerpT += EngineTime::GetInstance().GetDeltaTime();
		//
		transformComponent->SetPosition(glm::vec2{ newX,newY });
	}
}