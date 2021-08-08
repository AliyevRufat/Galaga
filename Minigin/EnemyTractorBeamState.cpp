#include "MiniginPCH.h"
#include "EnemyTractorBeamState.h"
#include "EnemyFormationState.h"
#include "BezierPath.h"
#include "TransformComponent.h"
#include "FormationManager.h"
#include "EnemyGoToFormationState.h"
#include "TractorBeamComponent.h"
#include "SceneManager.h"

EnemyTractorBeamState::EnemyTractorBeamState()
{
	m_SwitchState = false;
	m_pBezierPathManager = new BezierPathManager();
}

EnemyTractorBeamState::~EnemyTractorBeamState()
{
	delete m_pBezierPathManager;
}

void EnemyTractorBeamState::Update(EnemyStateManager& enemyStateManager)
{
	auto tractorBeamComp = enemyStateManager.GetGameObject()->GetComponent<TractorBeamComponent>();
	if (tractorBeamComp->GetIsFinished())
	{
		if (tractorBeamComp->GetIsPlayerCaught())//go to formation if player caught
		{
			m_SwitchState = true;
		}
		else //go straight down if player is not caught
		{
			auto transformComp = enemyStateManager.GetGameObject()->GetComponent<TransformComponent>();
			const float speed = 300;
			int newPosY = int(transformComp->GetTransform().GetPosition().y + speed * EngineTime::GetInstance().GetDeltaTime());
			//
			if (transformComp->GetTransform().GetPosition().y >= dae::SceneManager::GetInstance().GetScreenDimensions().y)
			{
				newPosY = 0;
				m_SwitchState = true;
			}
			//
			transformComp->SetPosition(glm::vec2(transformComp->GetTransform().GetPosition().x, newPosY));
		}
	}
}

void EnemyTractorBeamState::Enter(EnemyStateManager& enemyManager)
{
	enemyManager.GetGameObject()->GetComponent<TractorBeamComponent>()->CreateTractorBeam();
}

EnemyState* EnemyTractorBeamState::StateSwitch(EnemyStateManager& enemyStateMngr)
{
	EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies(enemyStateMngr.GetEnemyType());
	//
	return new EnemyGoToFormationState();
}

void EnemyTractorBeamState::CreatePaths(EnemyStateManager&)
{
}