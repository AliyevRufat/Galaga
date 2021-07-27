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
	virtual void CreatePaths() = 0;

	bool GetSwitchState() const { return m_SwitchState; }
protected:
	bool m_SwitchState;
	//
	std::vector<std::vector<glm::vec2>> m_Paths;
	int m_CurrentPath = 0;
	int m_CurrentWayPoint = 0;
	const float EPSILON = 5.0f;
};
