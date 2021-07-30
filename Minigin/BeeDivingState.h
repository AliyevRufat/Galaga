#pragma once
#include "BeeState.h"

class BeeDivingState final : public BeeState
{
public:
	BeeDivingState();
	~BeeDivingState();

	void Update(BeeStateManager& bee) override;
	void Enter(BeeStateManager& bee) override;
	virtual BeeState* StateSwitch() override;
	void CreatePaths(BeeStateManager& bee) override;
};
