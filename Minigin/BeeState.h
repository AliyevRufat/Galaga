#pragma once
#include "BeeStateManager.h"

class BeeState
{
public:
	BeeState() = default;
	virtual ~BeeState() = default;

	virtual void Update(BeeStateManager& bee) = 0;
	virtual BeeState* StateSwitch() = 0;
	virtual void Enter(BeeStateManager& bee) = 0;

	bool GetSwitchState() const;
protected:
	bool m_SwitchState;
};
