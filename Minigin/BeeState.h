#pragma once
#include "Bee.h"

class BeeState
{
public:
	BeeState() = default;
	virtual ~BeeState() = default;

	virtual void Update(Bee& bee) = 0;
	virtual BeeState* StateSwitch() = 0;
	virtual void Enter(Bee& bee) = 0;
};
