#include "MiniginPCH.h"
#include "ScoreObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "FPSTextComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "StageManager.h"

void ScoreObserver::OnNotify(const GameObject* actor, const std::string& event)
{
	if (event == "ScoreGained")
	{
		ChangeScore(actor);
	}
}

void ScoreObserver::ChangeScore(const GameObject* actor)
{
	std::shared_ptr<GameObject> spPointsDisplay = nullptr;
	if (actor->GetName() == "Gyaraga")
	{
		spPointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplay");
	}
	else
	{
		spPointsDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreDisplay2");
	}

	if (!spPointsDisplay) {
		std::cout << "WARNING: pointsDisplay not found after player death" << std::endl;
		return;
	}
	auto textComp = std::static_pointer_cast<GameObject>(spPointsDisplay).get()->GetComponent<TextComponent>();

	if (!textComp) {
		std::cout << "WARNING: TextComp of pointsDisplay not found after player death" << std::endl;
		return;
	}

	auto scoreComp = actor->GetComponent<ScoreComponent>();

	if (!scoreComp) {
		std::cout << "WARNING: ScoreComp not found after player death" << std::endl;
		return;
	}
	//
	if (StageManager::GetInstance().GetCurrentGameMode() == StageManager::GameMode::Coop)
	{
		if (actor->GetName() == "Gyaraga")
		{
			textComp->UpdateText("Score P1 : " + std::to_string(scoreComp->GetScore()));
		}
		else
		{
			textComp->UpdateText("Score P2 : " + std::to_string(scoreComp->GetScore()));
		}
	}
	else
	{
		textComp->UpdateText("Score: " + std::to_string(scoreComp->GetScore()));
	}
}