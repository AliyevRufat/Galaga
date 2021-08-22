#include "MiniginPCH.h"
#include "LivesObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "PlayerHealthComponent.h"
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

	auto healthComp = actor->GetComponent<PlayerHealthComponent>();

	if (!healthComp) {
		std::cout << "WARNING: HealthComp of Player0 not found after player death" << std::endl;
		return;
	}
	//
	int amountOfLives = healthComp->GetLives();
	if (amountOfLives != 0)
	{
		--amountOfLives;
	}

	if (currentGameMode == StageManager::GameMode::Coop)
	{
		if (actor->GetName() == "Gyaraga")
		{
			textComp->UpdateText("Lives P1 : " + std::to_string(amountOfLives));
		}
		else
		{
			textComp->UpdateText("Lives P2 : " + std::to_string(amountOfLives));
		}
	}
	else
	{
		textComp->UpdateText("Lives: " + std::to_string(amountOfLives));
	}

	//player died text
	std::shared_ptr<GameObject> spPlayerEvent = nullptr;

	std::string text;
	auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();

	if (currentGameMode == StageManager::GameMode::Coop)
	{
		if (((currentScene->GetPlayer(0) && currentScene->GetPlayer(0)->GetComponent<PlayerHealthComponent>()->GetIsDead()) && (currentScene->GetPlayer(1) && currentScene->GetPlayer(1)->GetComponent<PlayerHealthComponent>()->GetIsDead())))
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
		if ((currentScene->GetPlayer(0) && currentScene->GetPlayer(0)->GetComponent<PlayerHealthComponent>()->GetIsDead()))
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