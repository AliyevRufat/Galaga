#include "MiniginPCH.h"
#include "BeeStateManager.h"
#include "BeeFormationState.h"
#include "BeeSpawnState.h"
#include "EngineTime.h"

BeeStateManager::BeeStateManager(int formationPositionIndex)
	:m_IsInit{ false }
	, m_FormationIndex{ formationPositionIndex }
{
	m_pBeeState = new BeeSpawnState();
	m_pBeeState->Enter(*this);
	m_pBeeState->SetFormationIndex(formationPositionIndex);
}

BeeStateManager::~BeeStateManager()
{
	delete m_pBeeState;
}

void BeeStateManager::Update()
{
	if (!m_IsInit)
	{
		m_pBeeState->CreatePaths(*this);
		m_IsInit = true;
	}
	m_pBeeState->Update(*this);
	//timer for switching the state of the bee
	if (m_pBeeState->GetSwitchState())
	{
		StateSwitch();
	}
}

void BeeStateManager::StateSwitch()
{
	BeeState* state = m_pBeeState->StateSwitch();

	if (state != nullptr)
	{
		delete m_pBeeState;
		m_pBeeState = state;
		m_pBeeState->Enter(*this);
		m_pBeeState->SetFormationIndex(m_FormationIndex);
		m_pBeeState->CreatePaths(*this);
	}
}

GameObject* BeeStateManager::GetGameObject()
{
	return m_pGameObject;
}