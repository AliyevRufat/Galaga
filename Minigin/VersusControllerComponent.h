#pragma once
#include <SDL.h>
#include "BaseComponent.h"

class VersusControllerComponent final : public BaseComponent
{
public:
	VersusControllerComponent();
	//
	void Update();
	//
	bool GetIsTractorBeamActivated() const;
	bool GetIsDivingActivated() const;
	bool GetIsShootingActivated() const;
	//
	void SetIsTractorBeamActivated(bool isTractorBeamActivated);
	void SetIsDivingActivated(bool isDivingActivated);
	void SetIsShootingActivated(bool isShootingActivated);
private:

	bool m_IsTractorBeamActivated;
	bool m_IsDivingActivated;
	bool m_IsShootingActivated;
	//
	const int m_ActiveTime;
	float m_TractorBeamActiveTimer;
	float m_DiveActiveTimer;
	float m_ShootActiveTimer;
};
