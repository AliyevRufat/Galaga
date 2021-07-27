#include "MiniginPCH.h"
#include "BeeFormationState.h"
#include "BeeDivingState.h"
#include "FormationManager.h"

BeeFormationState::BeeFormationState()
	:m_TimerBeforeDiving{ rand() % 12 + 3 }
	, m_TimeBeforeDiving{ 0.0f }
{
	m_SwitchState = { false };
}

void BeeFormationState::Update(BeeStateManager&)
{
	m_TimeBeforeDiving += EngineTime::GetInstance().GetDeltaTime();

	if (m_TimeBeforeDiving >= m_TimerBeforeDiving)
	{
		m_TimeBeforeDiving -= m_TimeBeforeDiving;
		m_SwitchState = true;
	}
}

BeeState* BeeFormationState::StateSwitch()
{
	return new BeeDivingState();
}

void BeeFormationState::Enter(BeeStateManager&)
{
}