#include "MiniginPCH.h"
#include "EnemyFormationState.h"
#include "EnemyDivingState.h"
#include "FormationManager.h"
#include "TransformComponent.h"

EnemyFormationState::EnemyFormationState()
	:m_TimerBeforeDiving{ rand() % 12 + 3 }
	, m_TimeBeforeDiving{ 0.0f }
{
	m_SwitchState = { false };
}

void EnemyFormationState::Update(EnemyStateManager& enemyStateMngr)
{
	//timer before getting out of the formation
	m_TimeBeforeDiving += EngineTime::GetInstance().GetDeltaTime();

	if (m_TimeBeforeDiving >= 2)
	{
		m_TimeBeforeDiving -= m_TimeBeforeDiving;
		m_SwitchState = true;
	}
	//update pos in formation
	enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->SetPosition(FormationManager::GetInstance().GetSpecificPos(enemyStateMngr.GetGameObject()));
}

EnemyState* EnemyFormationState::StateSwitch(EnemyStateManager&)
{
	return new EnemyDivingState();
}

void EnemyFormationState::Enter(EnemyStateManager&)
{
}

void EnemyFormationState::CreatePaths(EnemyStateManager&)
{
}