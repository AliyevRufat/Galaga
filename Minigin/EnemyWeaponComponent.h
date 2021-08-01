#pragma once
#include "BaseComponent.h"
#include "BulletMovementComponent.h"
#include <SDL.h>

class EnemyWeaponComponent final : public BaseComponent
{
public:

	EnemyWeaponComponent(const int enemyWidth);
	void Update() override;
	void Shoot();

private:

	void CreateBullet();
	//
	const int m_EnemyWidth;
	std::vector<std::shared_ptr<GameObject>> m_spBullets;
};
