#pragma once
#include <SDL.h>
#include "BaseComponent.h"

class GyaragaMovementComponent final : public BaseComponent
{
public:

	GyaragaMovementComponent(SDL_Surface* windowSurface);
	void Update() override;
	//
	void Move(bool moveLeft);
	void StopMoving(bool moveLeft);

private:

	bool m_MoveLeft;
	bool m_MoveRight;
	SDL_Surface* m_WindowSurface;
};
