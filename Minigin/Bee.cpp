#include "MiniginPCH.h"
#include "Bee.h"
#include "BeeFormationState.h"
#include "EngineTime.h"

Bee::Bee()
	:m_StateSwitchTime{ 0.0f }
	, m_StateSwitchTimer{ rand() % 3 + 13 }
{
	m_BeeState = new BeeFormationState();
}

void Bee::Update()
{
	m_BeeState->Update(*this);
	//timer for switching the state of the bee
	if (m_StateSwitchTime <= m_StateSwitchTimer)
	{
		m_StateSwitchTime += EngineTime::GetInstance().GetDeltaTime();
	}
	else
	{
		m_StateSwitchTime -= m_StateSwitchTime;
		StateSwitch();
	}
}

void Bee::StateSwitch()
{
	BeeState* state = m_BeeState->StateSwitch();

	if (state != nullptr)
	{
		delete m_BeeState;
		m_BeeState = state;

		m_BeeState->Enter(*this);
	}
}