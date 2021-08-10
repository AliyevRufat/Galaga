#include "MiniginPCH.h"
#include "AccuracyObserver.h"
#include "SceneManager.h"
#include "Scene.h"
#include "HealthComponent.h"
#include "FPSTextComponent.h"
#include "PlayerWeaponComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"

void AccuracyObserver::OnNotify(const GameObject* actor, const std::string& event)
{
	if (event == "AccuracyUpdate")
	{
		ChangeAccuracy(actor);
	}
}

void AccuracyObserver::ChangeAccuracy(const GameObject* actor)
{
	std::shared_ptr<GameObject> spAccuracyDisplay = nullptr;
	if (actor->GetName() == "Gyaraga")
	{
		spAccuracyDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("AccuracyDisplay");
	}
	else
	{
		spAccuracyDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("AccuracyDisplay2");
	}

	if (!spAccuracyDisplay) {
		std::cout << "WARNING: spAccuracyDisplay not found after player death" << std::endl;
		return;
	}
	auto textComp = std::static_pointer_cast<GameObject>(spAccuracyDisplay).get()->GetComponent<TextComponent>();

	if (!textComp) {
		std::cout << "WARNING: TextComp of spAccuracyDisplay not found after player death" << std::endl;
		return;
	}

	auto weaponComp = actor->GetComponent<PlayerWeaponComponent>();

	if (!weaponComp) {
		std::cout << "WARNING: weaponComp not found after player death" << std::endl;
		return;
	}
	textComp->UpdateText("Accuracy: " + std::to_string(weaponComp->GetAccuracy()) + " %");
}