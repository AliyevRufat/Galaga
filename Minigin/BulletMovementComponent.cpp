#include "MiniginPCH.h"
#include "BulletMovementComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "EngineTime.h"
#include "LerpComponent.h"
#include "Locator.h"

BulletMovementComponent::BulletMovementComponent(const glm::vec2& target)
	:m_Target{ target }
{
	Locator::GetAudio().PlaySound("Fire", true);
}

void BulletMovementComponent::Init()
{
	if (m_Target != glm::vec2(0, 0))
	{
		auto currentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		const int targetOffset = 2;
		//
		auto BA = m_Target - currentPos;
		auto furtherTarget = glm::vec2(BA.x * targetOffset, BA.y * targetOffset);
		auto newTarget = glm::vec2(furtherTarget.x + currentPos.x, furtherTarget.y + currentPos.y);
		//
		m_pGameObject->AddComponent(new LerpComponent(newTarget, 150));
	}
}

void BulletMovementComponent::Update()
{
	if (m_Target != glm::vec2{ 0,0 })
	{
		return;
	}
	//shoot straight up
	auto transformComponent = m_pGameObject->GetComponent<TransformComponent>();
	auto currentPos = transformComponent->GetTransform().GetPosition();
	//
	const int speed = 1000;
	transformComponent->SetPosition(glm::vec2{ currentPos.x, currentPos.y - speed * EngineTime::GetInstance().GetDeltaTime() });
}