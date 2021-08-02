#pragma once
#include "ComponentIncludes.h"
#include "BezierPath.h"
#include "EnemyManager.h"

class GameObject;
class EnemyState;

class EnemyStateManager : public BaseComponent
{
public:
	EnemyStateManager(EnemyType enemyType, int formationRowIndex, int formationPositionIndex);
	~EnemyStateManager();
	void Update() override;
	GameObject* GetGameObject();
	EnemyType GetEnemyType() const;
	EnemyState* GetState()const;
private:

	void StateSwitch();
	//
	EnemyState* m_pEnemyState;
	EnemyType m_EnemyType;

	bool m_IsInit;
	int m_FormationIndex;
	int m_FormationRowIndex;
};
