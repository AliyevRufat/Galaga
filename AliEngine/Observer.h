#pragma once
#include "GameObject.h"

class Observer
{
public:
	virtual ~Observer() = default;
	virtual void OnNotify(const GameObject* actor, const std::string& event) = 0;
};
