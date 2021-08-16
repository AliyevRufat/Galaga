#pragma once
#include "ComponentIncludes.h"

class PlayerHealthComponent : public BaseComponent
{
public:
	PlayerHealthComponent(const unsigned int& health);
	//
	const unsigned int& GetLives() const;
	bool GetIsDead() const;
	void Die(bool explode = false);
private:
	unsigned int m_Lives;
	bool m_IsGameOver;
};