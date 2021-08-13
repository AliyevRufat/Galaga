#include "MiniginPCH.h"
#include "EnemyStateManager.h"
#include "EnemyFormationState.h"
#include "EnemySpawnState.h"
#include "EngineTime.h"

EnemyStateManager::EnemyStateManager(EnemyType enemyType, int formationRowIndex, int formationPositionIndex, int speed)
	:m_IsInit{ false }
	, m_EnemyType{ enemyType }
	, m_FormationIndex{ formationPositionIndex }
	, m_FormationRowIndex{ formationRowIndex }
	, m_Speed{ speed }
{
	m_pEnemyState = new EnemySpawnState();
	m_pEnemyState->SetFormationIndex(formationRowIndex, formationPositionIndex);
}

EnemyStateManager::~EnemyStateManager()
{
	delete m_pEnemyState;
}

void EnemyStateManager::Update()
{
	if (!m_IsInit)
	{
		m_pEnemyState->Enter(*this);
		m_IsInit = true;
	}
	m_pEnemyState->Update(*this);
	//timer for switching the state of the bee
	if (m_pEnemyState->GetSwitchState())
	{
		StateSwitch();
	}
}

void EnemyStateManager::StateSwitch()
{
	EnemyState* state = m_pEnemyState->StateSwitch(*this);

	if (state != nullptr)
	{
		delete m_pEnemyState;
		m_pEnemyState = state;
		m_pEnemyState->SetFormationIndex(m_FormationRowIndex, m_FormationIndex);
		m_pEnemyState->Enter(*this);
	}
}

GameObject* EnemyStateManager::GetGameObject()
{
	return m_pGameObject;
}

EnemyType EnemyStateManager::GetEnemyType() const
{
	return m_EnemyType;
}

EnemyState* EnemyStateManager::GetState()const
{
	return m_pEnemyState;
}