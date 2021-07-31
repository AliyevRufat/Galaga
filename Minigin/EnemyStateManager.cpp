#include "MiniginPCH.h"
#include "EnemyStateManager.h"
#include "EnemyFormationState.h"
#include "EnemySpawnState.h"
#include "EngineTime.h"
#include "EnemyManager.h"

EnemyStateManager::EnemyStateManager(EnemyType enemyType, int formationPositionIndex)
	:m_IsInit{ false }
	, m_EnemyType{ enemyType }
	, m_FormationIndex{ formationPositionIndex }
{
	m_pEnemyState = new EnemySpawnState();
	m_pEnemyState->Enter(*this);
	m_pEnemyState->SetFormationIndex(formationPositionIndex);
}

EnemyStateManager::~EnemyStateManager()
{
	delete m_pEnemyState;
}

void EnemyStateManager::Update()
{
	if (!m_IsInit)
	{
		m_pEnemyState->CreatePaths(*this);
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
		m_pEnemyState->Enter(*this);
		m_pEnemyState->SetFormationIndex(m_FormationIndex);
		m_pEnemyState->CreatePaths(*this);
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