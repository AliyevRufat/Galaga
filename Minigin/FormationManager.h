#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "BeeStateManager.h"

class TransformComponent;

class FormationManager final : public dae::Singleton<FormationManager>
{
public:
	void Update();
	void AddBeeToFormation(const std::shared_ptr<GameObject>& bee);
private:
	friend class dae::Singleton<FormationManager>;

	FormationManager() = default;

	std::vector<std::shared_ptr<GameObject>> m_pBees;
	const int m_TimerBeforeMovingToOtherSide = 3;
	float m_TimeBeforeMovingToOtherSide = 0.0f;
	int m_Speed = 50;
};
