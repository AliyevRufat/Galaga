#include "MiniginPCH.h"
#include "ParallaxBackgroundComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"

ParallaxBackgroundComponent::ParallaxBackgroundComponent(const std::string& pngPath, int speed)
	:m_Background1{ nullptr }
	, m_Background2{ nullptr }
	, m_Speed{ speed }
	, m_PathFile(pngPath)
{
}

void ParallaxBackgroundComponent::Init()
{
	m_Background1 = std::make_shared<GameObject>("Background");
	m_Background1->AddComponent(new TransformComponent(glm::vec2(0, 0)));
	m_Background1->AddComponent(new Texture2DComponent(m_PathFile));
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(m_Background1);
	//
	m_Background2 = std::make_shared<GameObject>("Background");
	m_Background2->AddComponent(new TransformComponent(glm::vec2(0, -dae::SceneManager::GetInstance().GetScreenDimensions().y)));
	m_Background2->AddComponent(new Texture2DComponent(m_PathFile));
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(m_Background2);
}

void ParallaxBackgroundComponent::Update()
{
	auto background1TransfComp = m_Background1->GetComponent<TransformComponent>();
	auto background2TransfComp = m_Background2->GetComponent<TransformComponent>();
	auto screenDimensions = dae::SceneManager::GetInstance().GetScreenDimensions();
	//if out of bounds bring back to spawn pos
	background1TransfComp->SetPosition(glm::vec2(0, (background1TransfComp->GetTransform().GetPosition().y + m_Speed * EngineTime::GetInstance().GetDeltaTime())));
	background2TransfComp->SetPosition(glm::vec2(0, (background2TransfComp->GetTransform().GetPosition().y + m_Speed * EngineTime::GetInstance().GetDeltaTime())));
	//
	if (background1TransfComp->GetTransform().GetPosition().y >= screenDimensions.y)
	{
		background1TransfComp->SetPosition(glm::vec2(0, -screenDimensions.y));
	}
	if (background2TransfComp->GetTransform().GetPosition().y >= screenDimensions.y)
	{
		background2TransfComp->SetPosition(glm::vec2(0, -screenDimensions.y));
	}
}