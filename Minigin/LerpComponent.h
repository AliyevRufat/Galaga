#pragma once
#include "ComponentIncludes.h"
#include <glm\vec2.hpp>

class LerpComponent : public BaseComponent
{
public:
	LerpComponent(const glm::vec2& endPos);
	//
	void Update() override;
	bool GetIsAtEndPos() const;
private:
	glm::vec2 m_EndPos;
	float m_LerpT;
	bool m_IsAtEndPos;
};
