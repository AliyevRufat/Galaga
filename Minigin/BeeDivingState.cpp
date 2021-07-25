#include "MiniginPCH.h"
#include "BeeDivingState.h"
#include "BeeFormationState.h"

BeeDivingState::BeeDivingState()
{
}

void BeeDivingState::Update(Bee&)
{
}

BeeState* BeeDivingState::StateSwitch()
{
	return new BeeFormationState();
}

void BeeDivingState::Enter(Bee&)
{
}