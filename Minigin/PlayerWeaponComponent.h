#pragma once
#include "BaseComponent.h"
#include "BulletMovementComponent.h"
#include <SDL.h>

class PlayerWeaponComponent final : public BaseComponent
{
public:
	PlayerWeaponComponent(const int playerWidth);
	//
	void Update() override;
	void Shoot();

private:
	void CreateBullet();
	//
	const int m_MaxBulletCountOnScreen;
	int m_CurrentBulletCountOnScreen;
	const int m_PlayerWidth;
	//
	std::vector<std::shared_ptr<GameObject>> m_spBullets;
};
