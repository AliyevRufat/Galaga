#pragma once
#include "BeeState.h"

class BeeSpawnState final : public BeeState
{
public:
	BeeSpawnState();

	void Update(BeeStateManager& bee) override;
	void Enter(BeeStateManager& bee) override;
	virtual BeeState* StateSwitch() override;
	void CreatePaths() override;
};
