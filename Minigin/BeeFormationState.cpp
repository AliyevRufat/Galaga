#include "MiniginPCH.h"
#include "BeeFormationState.h"
#include "BeeDivingState.h"

BeeFormationState::BeeFormationState()
{
}

void BeeFormationState::Update(Bee&)
{
}

BeeState* BeeFormationState::StateSwitch()
{
	return new BeeDivingState();
}

void BeeFormationState::Enter(Bee&)
{
}