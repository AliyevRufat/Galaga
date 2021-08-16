#pragma once
#include "ComponentIncludes.h"

class BossHealthComponent : public BaseComponent
{
public:
	BossHealthComponent(const unsigned int& health);
	//
	const unsigned int& GetLives() const;
	void Die();
private:
	unsigned int m_Lives;
};
