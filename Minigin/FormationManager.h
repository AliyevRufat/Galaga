#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "BeeStateManager.h"

class TransformComponent;

class FormationManager final : public dae::Singleton<FormationManager>
{
public:
	void Init();
	void Update();
	int GetAvailablePosInFormation();
	glm::vec2 GetPosWithIndex(int index);
	void ClearFormation();
private:
	friend class dae::Singleton<FormationManager>;

	FormationManager() = default;

	std::vector<std::pair<glm::vec2, bool>> m_BeePositions;

	const int m_TimerBeforeMovingToOtherSide = 3;
	float m_TimeBeforeMovingToOtherSide = 0.0f;
	int m_Speed = 15;
};
