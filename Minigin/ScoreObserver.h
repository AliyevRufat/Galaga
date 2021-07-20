#pragma once
#include "Observer.h"
#include "GameObject.h"

class ScoreObserver final : public Observer
{
public:
	void OnNotify(const GameObject* actor, const std::string& event) override;

private:
	void ChangeScore(const GameObject* actor);
};
