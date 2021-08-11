#include "MiniginPCH.h"
#include "TractorBeamComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Texture2DComponent.h"
#include "AnimationComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CollisionDetectionManager.h"
#include "EnemyWeaponComponent.h"
#include "LerpComponent.h"

TractorBeamComponent::TractorBeamComponent()
	:m_pTractorBeam{ nullptr }
	, m_IsFinished{ false }
	, m_IsCollisionAdded{ false }
	, m_IsPlayerCaught{ false }
{
}

void TractorBeamComponent::Update()
{
	UpdateTractorBeam();
	UpdateCaughtPlayer();
}

void TractorBeamComponent::UpdateTractorBeam()
{
	if (!m_IsFinished && m_pTractorBeam)
	{
		auto animationComp = m_pTractorBeam->GetComponent<AnimationComponent>();

		if (animationComp->GetCurrentRowIndex() == 1 && !m_IsCollisionAdded)
		{
			CollisionDetectionManager::GetInstance().AddCollisionGameObject(m_pTractorBeam); //tractor beam has reached player
			m_IsCollisionAdded = true;
		}
		else if (animationComp->GetCurrentRowIndex() == 2 && m_IsCollisionAdded)
		{
			CollisionDetectionManager::GetInstance().DeleteSpecificObject(m_pTractorBeam); //tractor beam is retrieving
			m_IsCollisionAdded = false;
		}
		//
		if (animationComp->GetCurrentRowIndex() == 2 && animationComp->GetCurrentFrame() == 10)
		{
			m_IsFinished = true;
			m_pTractorBeam = nullptr;
		}
	}
}

void TractorBeamComponent::UpdateCaughtPlayer()
{
	if (m_pCaughtPlayer)
	{
		if (m_pCaughtPlayer->GetComponent<LerpComponent>()->GetIsAtEndPos())
		{
			auto parentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			const int caughtPlayerHeight = 43;
			const int widthOffset = 5;
			//
			m_pCaughtPlayer->GetComponent<TransformComponent>()->SetPosition(glm::vec2(parentPos.x + widthOffset, parentPos.y - caughtPlayerHeight));
		}
	}
}

void TractorBeamComponent::CreateTractorBeam()
{
	const int offset = 28;
	auto parentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	glm::vec2 tractorBeamScale = glm::vec2(110, 260);
	//
	m_pTractorBeam = std::make_shared<GameObject>("TractorBeam", m_pGameObject);
	m_pTractorBeam->AddComponent(new TransformComponent(glm::vec2(parentPos.x - offset, parentPos.y), tractorBeamScale));
	m_pTractorBeam->AddComponent(new Texture2DComponent("TractorBeam.png", 1, true));
	m_pTractorBeam->AddComponent(new AnimationComponent(0.2f, 10, 3, false));
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(m_pTractorBeam);
	m_IsFinished = false;
}

bool TractorBeamComponent::GetIsFinished() const
{
	return m_IsFinished;
}

void TractorBeamComponent::Clean()
{
	if (m_pTractorBeam)
	{
		m_pTractorBeam->SetMarkForDelete(true);
	}
	if (m_pCaughtPlayer)
	{
		m_pCaughtPlayer->SetMarkForDelete(true);
	}
}

bool TractorBeamComponent::GetIsPlayerCaught() const
{
	return m_IsPlayerCaught;
}

void TractorBeamComponent::SpawnAFighter(const glm::vec2& playerPos)
{
	const int caughtPlayerWidth = 45;
	const int caughtPlayerHeight = 43;
	const int widthOffset = 5;
	auto parentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	//
	m_pCaughtPlayer = std::make_shared<GameObject>("CaughtFighter", m_pGameObject, glm::vec2(caughtPlayerWidth, caughtPlayerHeight));
	m_pCaughtPlayer->AddComponent(new TransformComponent(playerPos, glm::vec2(caughtPlayerWidth, caughtPlayerHeight)));
	m_pCaughtPlayer->AddComponent(new Texture2DComponent("CaughtGyaraga.png", 1, false));
	m_pCaughtPlayer->AddComponent(new EnemyWeaponComponent(true));
	m_pCaughtPlayer->AddComponent(new LerpComponent(glm::vec2(parentPos.x + widthOffset, parentPos.y - caughtPlayerHeight)));
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(m_pCaughtPlayer);
	//
	m_IsPlayerCaught = true;
}