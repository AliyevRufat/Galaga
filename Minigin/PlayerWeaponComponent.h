#pragma once
#include "BaseComponent.h"
#include "BulletMovementComponent.h"
#include <SDL.h>

class PlayerWeaponComponent final : public BaseComponent
{
public:
	PlayerWeaponComponent();
	//
	void Update() override;
	void Shoot();
	int GetAccuracy() const;
private:
	void CreateBullet();
	//
	std::vector<std::shared_ptr<GameObject>> m_spBullets;
	//
	const int m_MaxBulletCountOnScreen;
	int m_CurrentBulletCountOnScreen;
	//
	int m_AmountOfShotBullets;
	int m_AmountOfMissedBullets;
};
