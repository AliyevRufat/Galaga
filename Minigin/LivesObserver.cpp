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

void LivesObserver::OnNotify(const GameObject* actor, const std::string& event)
{
	if (event == "PlayerDied")
	{
		ChangeLives(actor);
	}
}

void LivesObserver::ChangeLives(const GameObject* actor)
{
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
	textComp->UpdateText("Remaining lives: " + std::to_string(healthComp->GetLives()));
	//player died text
	std::shared_ptr<GameObject> spPlayerDiedGO = nullptr;
	if (actor->GetName() == "Gyaraga")
	{
		spPlayerDiedGO = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("Player 1 Died!");
	}
	else
	{
		spPlayerDiedGO = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("Player 2 Died!");
	}
	auto textCompPlayerDied = std::static_pointer_cast<GameObject>(spPlayerDiedGO).get()->GetComponent<TextComponent>();
	textCompPlayerDied->SetIsVisible(true, 2.0f);
}