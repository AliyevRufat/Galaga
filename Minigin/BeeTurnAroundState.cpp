#include "MiniginPCH.h"
#include "BeeTurnAroundState.h"
#include "BeeDivingState.h"

BeeTurnAroundState::BeeTurnAroundState()
{
	m_SwitchState = { false };
}

void BeeTurnAroundState::Update(BeeStateManager&)
{
}

BeeState* BeeTurnAroundState::StateSwitch()
{
	return new BeeDivingState();;
}

void BeeTurnAroundState::Enter(BeeStateManager&)
{
}