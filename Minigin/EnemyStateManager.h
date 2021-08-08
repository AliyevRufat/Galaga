#pragma once
#include "ComponentIncludes.h"
#include "BezierPath.h"
#include "EnemyManager.h"

class GameObject;
class EnemyState;

class EnemyStateManager final : public BaseComponent
{
public:
	EnemyStateManager(EnemyType enemyType, int formationRowIndex, int formationPositionIndex);
	~EnemyStateManager();
	EnemyStateManager(const EnemyStateManager& other) = delete;
	EnemyStateManager(EnemyStateManager&& other) = delete;
	EnemyStateManager& operator=(const EnemyStateManager& other) = delete;
	EnemyStateManager& operator=(EnemyStateManager&& other) = delete;
	//
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
