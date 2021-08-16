#include "MiniginPCH.h"
#include "LivesObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "HealthComponent.h"
#include "FPSTextComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "StageManager.h"

void LivesObserver::OnNotify(const GameObject* actor, const std::string& event)
{
	if (event == "PlayerDied")
	{
		ChangeLives(actor);
	}
}

void LivesObserver::ChangeLives(const GameObject* actor)
{
	auto currentGameMode = StageManager::GetInstance().GetCurrentGameMode();
	std::shared_ptr<GameObject> spLivesDisplay = nullptr;
	if (actor->GetName() == "Gyaraga")
	{
		spLivesDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("LivesDisplay");
	}
	else
	{
		spLivesDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("LivesDisplay2");
	}

	if (!spLivesDisplay) {
		std::cout << "WARNING: livesDisplay not found after player death" << std::endl;
		return;
	}
	auto textComp = std::static_pointer_cast<GameObject>(spLivesDisplay).get()->GetComponent<TextComponent>();

	if (!textComp) {
		std::cout << "WARNING: TextComp of LiveCounter not found after player death" << std::endl;
		return;
	}

	auto healthComp = actor->GetComponent<HealthComponent>();

	if (!healthComp) {
		std::cout << "WARNING: HealthComp of Player0 not found after player death" << std::endl;
		return;
	}
	if (currentGameMode == StageManager::GameMode::Coop)
	{
		if (actor->GetName() == "Gyaraga")
		{
			textComp->UpdateText("Lives P1 : " + std::to_string(healthComp->GetLives() - 1));
		}
		else
		{
			textComp->UpdateText("Lives P2 : " + std::to_string(healthComp->GetLives() - 1));
		}
	}
	else
	{
		textComp->UpdateText("Lives: " + std::to_string(healthComp->GetLives()));
	}

	//player died text
	std::shared_ptr<GameObject> spPlayerEvent = nullptr;

	std::string text;
	auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();

	if (currentGameMode == StageManager::GameMode::Coop)
	{
		if (((currentScene->GetPlayer(0) && currentScene->GetPlayer(0)->GetComponent<HealthComponent>()->GetIsDead()) && (currentScene->GetPlayer(1) && currentScene->GetPlayer(1)->GetComponent<HealthComponent>()->GetIsDead())) || (currentScene->GetPlayer(1) && currentScene->GetPlayer(1)->GetComponent<HealthComponent>()->GetIsDead()))
		{
			text = "GAMEOVER";
		}
		else
		{
			return;
		}
	}
	else
	{
		if ((currentScene->GetPlayer(0) && currentScene->GetPlayer(0)->GetComponent<HealthComponent>()->GetIsDead()))
		{
			text = "GAMEOVER";
		}
		else
		{
			text = "READY";
		}
	}
	//
	spPlayerEvent = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName(text);
	auto textCompPlayerDied = std::static_pointer_cast<GameObject>(spPlayerEvent).get()->GetComponent<TextComponent>();
	textCompPlayerDied->SetIsVisible(true, 5.0f);
}