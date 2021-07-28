#pragma once
#include "BeeState.h"

class BeeTurnAroundState final : public BeeState
{
public:
	BeeTurnAroundState();

	void Update(BeeStateManager& bee) override;
	void Enter(BeeStateManager& bee) override;
	virtual BeeState* StateSwitch() override;
	void CreatePaths(BeeStateManager& bee) override;
};
