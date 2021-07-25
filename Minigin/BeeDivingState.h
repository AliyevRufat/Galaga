#pragma once
#include "BeeState.h"

class BeeDivingState final : public BeeState
{
public:
	BeeDivingState();

	void Update(Bee& bee) override;
	virtual BeeState* StateSwitch();
	void Enter(Bee& bee);
};
