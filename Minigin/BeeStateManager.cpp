#include "MiniginPCH.h"
#include "BeeStateManager.h"
#include "BeeFormationState.h"
#include "BeeSpawnState.h"
#include "EngineTime.h"

BeeStateManager::BeeStateManager()
{
	m_pBeeState = new BeeSpawnState();
	m_pBeeState->Enter(*this);
}

BeeStateManager::~BeeStateManager()
{
	delete m_pBeeState;
}

void BeeStateManager::Update()
{
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
	}
}

GameObject* BeeStateManager::GetGameObject()
{
	return m_pGameObject;
}