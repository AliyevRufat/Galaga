#include "MiniginPCH.h"
#include "GyaragaMovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "EngineTime.h"

GyaragaMovementComponent::GyaragaMovementComponent(SDL_Surface* windowSurface)
	: m_MoveLeft{ false }
	, m_MoveRight{ false }
	, m_WindowSurface{ windowSurface }
{
}

void GyaragaMovementComponent::Update()
{
	auto transformComponent = m_pGameObject->GetComponent<TransformComponent>();
	//restrict movement on the sides
	const int playerWidth = 45;
	if (transformComponent->GetTransform().GetPosition().x <= 0)
	{
		m_MoveLeft = false;
	}
	if (transformComponent->GetTransform().GetPosition().x >= m_WindowSurface->w - playerWidth)
	{
		m_MoveRight = false;
	}
	//
	if (m_MoveLeft && m_MoveRight)
	{
		return;
	}
	else if (m_MoveLeft || m_MoveRight)
	{
		int speed = 500;
		//
		if (m_MoveLeft) //reverse sign if go to left
		{
			speed *= -1;
		}
		//
		transformComponent->SetPosition(glm::vec2(transformComponent->GetTransform().GetPosition().x + speed * EngineTime::GetInstance().GetDeltaTime(), transformComponent->GetTransform().GetPosition().y));
	}
}

void GyaragaMovementComponent::Move(bool moveLeft)
{
	if (moveLeft)
	{
		m_MoveLeft = true;
	}
	else
	{
		m_MoveRight = true;
	}
}

void GyaragaMovementComponent::StopMoving(bool moveLeft)
{
	if (moveLeft)
	{
		m_MoveLeft = false;
	}
	else
	{
		m_MoveRight = false;
	}
}