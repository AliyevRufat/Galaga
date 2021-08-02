#pragma once
#include "BaseComponent.h"
#include <glm\vec2.hpp>

class BulletMovementComponent final : public BaseComponent
{
public:

	BulletMovementComponent(const glm::vec2& target = glm::vec2{ 0,0 });
	void Init();
	void Update() override;
private:
	glm::vec2 m_Target;
	glm::vec2 m_TargetOffset;
	float m_LerpT;
};
