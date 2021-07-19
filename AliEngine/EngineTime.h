#pragma once
#include "Singleton.h"

class EngineTime final : public dae::Singleton<EngineTime>
{
public:
	float GetDeltaTime();
	void SetDeltaTime(float deltaTime);
private:
	EngineTime() = default;
	float m_DeltaTime{};
	friend class dae::Singleton<EngineTime>;
};