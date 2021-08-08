#pragma once
#include <SDL.h>
#include "BaseComponent.h"

class GyaragaMovementComponent final : public BaseComponent
{
public:
	GyaragaMovementComponent(int playerWidth);
	//
	void Update() override;
	void Move(bool moveLeft);
	void StopMoving(bool moveLeft);

private:
	int m_PlayerWidth;
	bool m_MoveLeft;
	bool m_MoveRight;
};
