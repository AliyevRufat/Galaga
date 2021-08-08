#pragma once
#include "BaseComponent.h"
#include "BulletMovementComponent.h"
#include <SDL.h>

class EnemyWeaponComponent final : public BaseComponent
{
public:

	EnemyWeaponComponent(const int enemyWidth, bool isAutoFire = false);
	void Update() override;
	void Shoot();

private:
	void HandleAutoFire();
	void CreateBullet();
	//
	const int m_EnemyWidth;
	std::vector<std::shared_ptr<GameObject>> m_spBullets;
	bool m_IsAutoFire;
	float m_AutoFireTime;
	const float m_AutoFireTimer;
};
