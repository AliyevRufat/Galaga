#pragma once
#include "ComponentIncludes.h"
#include "BezierPath.h"

class GameObject;
class BeeState;

class BeeStateManager : public BaseComponent
{
public:
	BeeStateManager(int formationPositionIndex);
	~BeeStateManager();
	void Update() override;
	GameObject* GetGameObject();
private:

	void StateSwitch();
	bool m_IsInit;
	BeeState* m_pBeeState;
	int m_FormationIndex;
};
