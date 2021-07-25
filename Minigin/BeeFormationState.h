#pragma once
#include "BeeState.h"
class BeeFormationState final : public BeeState
{
public:
	BeeFormationState();

	void Update(Bee& bee) override;
	virtual BeeState* StateSwitch();
	void Enter(Bee& bee);
};
