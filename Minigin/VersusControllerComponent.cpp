#include "MiniginPCH.h"
#include "VersusControllerComponent.h"
#include "../AliEngine/GameObject.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/EngineTime.h"
#include "TractorBeamComponent.h"

VersusControllerComponent::VersusControllerComponent()
	:m_IsTractorBeamActivated{ false }
	, m_IsDivingActivated{ false }
	, m_IsShootingActivated{ false }
	, m_ActiveTime{ 1 }
	, m_TractorBeamActiveTimer{ 0.0f }
	, m_DiveActiveTimer{ 0.0f }
	, m_ShootActiveTimer{ 0.0f }
{
}

void VersusControllerComponent::Update()
{
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();
	//tractorBeam
	if (m_IsTractorBeamActivated)
	{
		m_TractorBeamActiveTimer += deltaTime;

		if (m_TractorBeamActiveTimer >= m_ActiveTime)
		{
			m_TractorBeamActiveTimer -= m_TractorBeamActiveTimer;
			m_IsTractorBeamActivated = false;
		}
	}
	//dive
	if (m_IsDivingActivated)
	{
		m_DiveActiveTimer += deltaTime;

		if (m_DiveActiveTimer >= m_ActiveTime)
		{
			m_DiveActiveTimer -= m_DiveActiveTimer;
			m_IsDivingActivated = false;
		}
	}
	//shoot
	if (m_IsShootingActivated)
	{
		m_ShootActiveTimer += deltaTime;

		if (m_ShootActiveTimer >= m_ActiveTime)
		{
			m_ShootActiveTimer -= m_ShootActiveTimer;
			m_IsShootingActivated = false;
		}
	}
}

bool VersusControllerComponent::GetIsTractorBeamActivated() const
{
	if (m_pGameObject->GetComponent<TractorBeamComponent>()->GetIsPlayerCaught())
	{
		return false;
	}
	return m_IsTractorBeamActivated;
}

bool VersusControllerComponent::GetIsDivingActivated() const
{
	return m_IsDivingActivated;
}

bool VersusControllerComponent::GetIsShootingActivated() const
{
	return m_IsShootingActivated;
}

void VersusControllerComponent::SetIsTractorBeamActivated(bool isTractorBeamActivated)
{
	m_IsTractorBeamActivated = isTractorBeamActivated;
}

void VersusControllerComponent::SetIsDivingActivated(bool isDivingActivated)
{
	m_IsDivingActivated = isDivingActivated;
}

void VersusControllerComponent::SetIsShootingActivated(bool isShootingActivated)
{
	m_IsShootingActivated = isShootingActivated;
}