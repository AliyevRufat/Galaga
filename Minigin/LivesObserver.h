#pragma once
#include "../AliEngine/Observer.h"
#include "../AliEngine/GameObject.h"

class LivesObserver final : public Observer
{
public:
	void OnNotify(const GameObject* actor, const std::string& event) override;

private:
	void ChangeLives(const GameObject* actor);
};
