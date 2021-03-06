#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "EnemyStateManager.h"

class GameObject;
class TransformComponent;

class FormationManager final : public dae::Singleton<FormationManager>
{
public:
	void InitFormation();
	void Update();
	glm::vec2 GetSpecificPos(int rowIndex, int posIndex, EnemyType enemyType) const;

private:
	friend class dae::Singleton<FormationManager>;
	FormationManager() = default;
	//
	std::string m_FilePath;
	void ParsePosOfEnemy(const std::string& oneLine, int level, char firstLetter, char secondLetter, int row);
	void ReadFormationFromFile();
	//first vector is stage, second is the row and third is the columns
	std::vector<std::vector<std::vector<glm::vec2>>> m_BeePositions;
	std::vector<std::vector<std::vector<glm::vec2>>> m_ButterflyPositions;
	std::vector<std::vector<std::vector<glm::vec2>>> m_BossPositions;
	//
	const int m_TimerBeforeMovingToOtherSide = 3;
	float m_TimeBeforeMovingToOtherSide = 0.0f;
	//
	int m_Speed = 15;
	bool m_IsInitialized = false;
};
