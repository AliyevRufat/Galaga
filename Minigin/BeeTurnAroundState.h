#pragma once
#include "BeeState.h"

class BeeTurnAroundState final : public BeeState
{
public:
	BeeTurnAroundState();

	void Update(BeeStateManager& bee) override;
	virtual BeeState* StateSwitch();
	void Enter(BeeStateManager& bee);
};
