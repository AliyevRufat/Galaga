#pragma once
#include "../AliEngine/Observer.h"
#include "../AliEngine/GameObject.h"

class AccuracyObserver final : public Observer
{
public:
	void OnNotify(const GameObject* actor, const std::string& event) override;

private:
	void ChangeAccuracy(const GameObject* actor);
};
