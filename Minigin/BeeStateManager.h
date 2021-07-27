#pragma once
#include "ComponentIncludes.h"
#include "BezierPath.h"

class GameObject;
class BeeState;

class BeeStateManager : public BaseComponent
{
public:
	BeeStateManager();
	~BeeStateManager();
	void Update() override;
	GameObject* GetGameObject();
private:

	void StateSwitch();

	BeeState* m_pBeeState;
};
