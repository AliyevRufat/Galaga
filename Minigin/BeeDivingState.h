#pragma once
#include "BeeState.h"

class BeeDivingState final : public BeeState
{
public:
	BeeDivingState();

	void Update(BeeStateManager& bee) override;
	virtual BeeState* StateSwitch();
	void Enter(BeeStateManager& bee);
};
