#include "MiniginPCH.h"
#include "BeeFormationState.h"
#include "BeeDivingState.h"
#include "FormationManager.h"
#include "TransformComponent.h"

BeeFormationState::BeeFormationState()
	:m_TimerBeforeDiving{ rand() % 12 + 3 }
	, m_TimeBeforeDiving{ 0.0f }
	, m_Index{ 0 }
	, m_IsInit{ false }
{
	m_SwitchState = { false };
}

void BeeFormationState::Update(BeeStateManager& bee)
{
	//timer before getting out of the formation
	m_TimeBeforeDiving += EngineTime::GetInstance().GetDeltaTime();

	if (m_TimeBeforeDiving >= m_TimerBeforeDiving)
	{
		m_TimeBeforeDiving -= m_TimeBeforeDiving;
		m_SwitchState = true;
	}
	//staying in formation
	if (!m_IsInit)
	{
		m_Index = FormationManager::GetInstance().GetAvailablePosInFormation();
		m_IsInit = true;
	}
	bee.GetGameObject()->GetComponent<TransformComponent>()->SetPosition(FormationManager::GetInstance().GetPosWithIndex(m_Index));
}

BeeState* BeeFormationState::StateSwitch()
{
	return new BeeDivingState();
}

void BeeFormationState::Enter(BeeStateManager&)
{
}

void BeeFormationState::CreatePaths()
{
}