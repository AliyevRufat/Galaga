#include "MiniginPCH.h"
#include "EnemyDivingState.h"
#include "TransformComponent.h"
#include "EnemyFormationState.h"
#include "FormationManager.h"
#include "SceneManager.h"
#include "GoToFormationState.h"

EnemyDivingState::EnemyDivingState()
{
	m_SwitchState = { false };
	m_pBezierPathManager = new BezierPathManager();
	EnemyManager::GetInstance().IncreaseAmountOfDivingEnemies();
}

EnemyDivingState::~EnemyDivingState()
{
	delete m_pBezierPathManager;
}

void EnemyDivingState::Update(EnemyStateManager& enemyStateMngr)
{
	auto transformComp = enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>();
	glm::vec2 newPos = m_pBezierPathManager->CalculatePath(transformComp->GetTransform().GetPosition());
	//boundary check for butterfly
	if (enemyStateMngr.GetEnemyType() == EnemyType::Butterfly)
	{
		if (transformComp->GetTransform().GetPosition().y >= dae::SceneManager::GetInstance().GetScreenDimensions().y)
		{
			newPos.y = 0;
			m_SwitchState = true;
		}
	}
	//update pos on bezier path
	if (newPos != glm::vec2{ 0,0 })
	{
		transformComp->SetPosition(newPos);
	}
	else
	{
		m_SwitchState = true;
	}
}

EnemyState* EnemyDivingState::StateSwitch(EnemyStateManager& enemyStateMngr)
{
	EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies();
	//
	if (enemyStateMngr.GetEnemyType() == EnemyType::Butterfly)
	{
		return new GoToFormationState();
	}
	return new EnemyFormationState();
}

void EnemyDivingState::Enter(EnemyStateManager&)
{
}

void EnemyDivingState::CreatePaths(EnemyStateManager& enemyStateMngr)
{
	auto screenDimensions = dae::SceneManager::GetInstance().GetScreenDimensions();
	auto startPos = enemyStateMngr.GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	BezierPath* path = new BezierPath();
	const int formationMiddleIndex = 5;
	const int amountOfSamplesOnACurve = 30;

	switch (enemyStateMngr.GetEnemyType())
	{
	case EnemyType::Bee:
	{
		int randomDiveX = rand() % (int)screenDimensions.x + 1;
		const int offsetY = 100;
		auto curvePoint = glm::vec2(randomDiveX, screenDimensions.y + offsetY);
		//
		if (m_FormationPosIndex < formationMiddleIndex)
		{
			path->AddCurve({ startPos, glm::vec2(startPos.x - 320,startPos.y - 110), glm::vec2(curvePoint.x + 620 ,curvePoint.y),curvePoint }, amountOfSamplesOnACurve);
			path->AddCurve({ curvePoint, glm::vec2(curvePoint.x - 220,curvePoint.y - 400) ,glm::vec2(curvePoint.x + 220,curvePoint.y - 400),curvePoint }, amountOfSamplesOnACurve);
			path->AddCurve({ curvePoint, glm::vec2(curvePoint.x - 120,curvePoint.y - 180) ,glm::vec2(startPos.x - 70,startPos.y + 130),startPos }, amountOfSamplesOnACurve);
		}
		else
		{
			path->AddCurve({ startPos, glm::vec2(startPos.x + 320,startPos.y - 110), glm::vec2(curvePoint.x - 620 ,curvePoint.y),curvePoint }, amountOfSamplesOnACurve);
			path->AddCurve({ curvePoint, glm::vec2(curvePoint.x + 220,curvePoint.y - 400) ,glm::vec2(curvePoint.x - 220,curvePoint.y - 400),curvePoint }, amountOfSamplesOnACurve);
			path->AddCurve({ curvePoint, glm::vec2(curvePoint.x + 120,curvePoint.y - 180) ,glm::vec2(startPos.x + 70,startPos.y + 130),startPos }, amountOfSamplesOnACurve);
		}
	}
	break;

	case EnemyType::Butterfly:
	{
		int range = 0;
		int randTargetPoint = 0;
		const int amountOfBezierCurves = 4;
		const int distanceOnY = 200;
		auto endPos = startPos;

		if (m_FormationPosIndex < formationMiddleIndex)
		{
			range = (int)screenDimensions.x - (int)screenDimensions.x / 2 + 1;
			randTargetPoint = rand() % range + (int)screenDimensions.x / 2;
			//calculate endpos based on the distance
			const int distanceOnX = (randTargetPoint - (int)startPos.x) / amountOfBezierCurves;
			endPos.x += distanceOnX;
			endPos.y += distanceOnY;
			for (size_t i = 0; i < amountOfBezierCurves; i++)
			{
				path->AddCurve({ startPos, glm::vec2(startPos.x - distanceOnX, endPos.y), glm::vec2(endPos.x + distanceOnX ,startPos.y),endPos }, amountOfSamplesOnACurve);
				//
				startPos = endPos;
				endPos.x += distanceOnX;
				endPos.y += distanceOnY;
			}
		}
		else
		{
			range = (int)screenDimensions.x / 2 - 0 + 1;
			randTargetPoint = rand() % range + 0;
			//calculate endpos based on the distance
			const int distanceOnX = (randTargetPoint - (int)startPos.x) / amountOfBezierCurves;
			endPos.x += distanceOnX;
			endPos.y += distanceOnY;
			for (size_t i = 0; i < amountOfBezierCurves; i++)
			{
				path->AddCurve({ startPos, glm::vec2(startPos.x - distanceOnX, endPos.y), glm::vec2(endPos.x + distanceOnX ,startPos.y),endPos }, amountOfSamplesOnACurve);
				//
				startPos = endPos;
				endPos.x += distanceOnX;
				endPos.y += distanceOnY;
			}
		}
	}
	break;
	}
	//
	m_pBezierPathManager->CreatePaths(path);
	delete path;
}