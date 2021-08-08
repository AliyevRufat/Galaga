#include "MiniginPCH.h"
#include "EnemyDivingState.h"
#include "TransformComponent.h"
#include "EnemyFormationState.h"
#include "FormationManager.h"
#include "SceneManager.h"
#include "EnemyGoToFormationState.h"
#include "EnemyManager.h"
#include "EnemyWeaponComponent.h"

EnemyDivingState::EnemyDivingState(EnemyStateManager& enemyStateMngr)
{
	m_SwitchState = { false };
	m_pBezierPathManager = new BezierPathManager();
	EnemyManager::GetInstance().IncreaseAmountOfDivingEnemies(enemyStateMngr.GetEnemyType());
	//
	m_SwitchState = { false };
	//if can shoot
	const int randNr = rand() % 10 + 1;
	if (randNr <= EnemyManager::GetInstance().GetEnemyChanceToShoot())
	{
		m_CanShoot = true;
	}
	else
	{
		m_CanShoot = false;
	}
	m_ShootTimer = 0.3f;
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
	if (enemyStateMngr.GetEnemyType() == EnemyType::Butterfly || enemyStateMngr.GetEnemyType() == EnemyType::Boss)
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
	//
	ShootBullet(enemyStateMngr);
}

EnemyState* EnemyDivingState::StateSwitch(EnemyStateManager& enemyStateMngr)
{
	EnemyManager::GetInstance().DecreaseAmountOfDivingEnemies(enemyStateMngr.GetEnemyType());
	//
	if (enemyStateMngr.GetEnemyType() == EnemyType::Butterfly || enemyStateMngr.GetEnemyType() == EnemyType::Boss)
	{
		return new EnemyGoToFormationState();
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
		const int offsetY = 40;
		auto curvePoint = glm::vec2(randomDiveX, screenDimensions.y - offsetY);
		int mutiplier = 1;
		//
		if (m_FormationPosIndex < formationMiddleIndex)
		{
			mutiplier *= -1;
		}
		path->AddCurve({ startPos, glm::vec2(startPos.x + (100 * mutiplier),startPos.y - 150), glm::vec2(startPos.x + (150 * mutiplier),startPos.y + 100),glm::vec2(startPos.x,startPos.y + 200) }, amountOfSamplesOnACurve);
		path->AddCurve({ glm::vec2(startPos.x,startPos.y + 200), glm::vec2(startPos.x - (200 * mutiplier),startPos.y + 250) ,glm::vec2(curvePoint.x - (200 * mutiplier),curvePoint.y - 30),curvePoint }, amountOfSamplesOnACurve);
		path->AddCurve({ curvePoint, glm::vec2(curvePoint.x + (150 * mutiplier),curvePoint.y) ,glm::vec2(startPos.x + (60 * mutiplier),startPos.y + 200),startPos }, amountOfSamplesOnACurve);
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
		}
		else
		{
			range = (int)screenDimensions.x / 2 - 0 + 1;
			randTargetPoint = rand() % range + 0;
		}
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
	break;

	case EnemyType::Boss:
	{
		const int offset = 200;
		const int randomDiveX = rand() % ((int)screenDimensions.x - offset * 2) + offset;
		//
		path->AddCurve({ startPos, glm::vec2(startPos.x - 100,startPos.y - 250), glm::vec2(randomDiveX - 400,startPos.y + 240), glm::vec2(randomDiveX - 140,startPos.y + 240) }, amountOfSamplesOnACurve);
		path->AddCurve({ glm::vec2(randomDiveX - 140,startPos.y + 240), glm::vec2(randomDiveX + 160,startPos.y + 240),glm::vec2(randomDiveX - 450 ,-150) ,glm::vec2(randomDiveX,int(screenDimensions.y + 200)) }, amountOfSamplesOnACurve);
	}
	break;
	}
	//
	m_pBezierPathManager->CreatePaths(path);
	delete path;
}