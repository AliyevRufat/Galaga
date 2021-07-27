#pragma once
#include "BeeState.h"
class BeeFormationState final : public BeeState
{
public:
	BeeFormationState();

	void Update(BeeStateManager& bee) override;
	void Enter(BeeStateManager& bee) override;
	virtual BeeState* StateSwitch() override;
	void CreatePaths() override;
private:

	const int m_TimerBeforeDiving;
	float m_TimeBeforeDiving;

	int m_Index;
	bool m_IsInit;
};
