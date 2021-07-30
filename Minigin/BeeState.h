#pragma once
#include "BeeStateManager.h"
#include "BezierPathManager.h"

class BeeState
{
public:
	BeeState() = default;
	virtual ~BeeState() = default;

	virtual void Update(BeeStateManager& bee) = 0;
	virtual BeeState* StateSwitch() = 0;
	virtual void Enter(BeeStateManager& bee) = 0;
	virtual void CreatePaths(BeeStateManager& bee) = 0;
	void SetFormationIndex(int formationIndex) { m_FormationIndex = formationIndex; }

	bool GetSwitchState() const { return m_SwitchState; }
protected:
	bool m_SwitchState = false;
	int m_FormationIndex = 0;
	//
	BezierPathManager* m_pBezierPathManager = nullptr;
};
