#pragma once

class BeeState;

class Bee
{
public:
	Bee();
	void Update();

private:

	void StateSwitch();

	BeeState* m_BeeState;

	const int m_StateSwitchTimer;
	float m_StateSwitchTime;
};
