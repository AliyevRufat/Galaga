#pragma once
#include <iostream>
#include "../AliEngine/BaseCommand.h"
#include "Components/HealthComponent.h"
#include "../AliEngine/Components/FPSTextComponent.h"
#include "Components/ScoreComponent.h"
#include "../AliEngine/Components/TextComponent.h"
#include "../AliEngine/Components/Texture2DComponent.h"
#include "../AliEngine/Components/TransformComponent.h"
#include "Components/AnimationComponent.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/Input/InputManager.h"

class ExitCommand final : public Command
{
public:
	ExitCommand(int index) :Command(index) {};

	void Execute() const override { dae::InputManager::GetInstance().SetHasExited(true); }
	void Release() const override {};
	void Undo() override {};
};

//-------------------------------------------------------------------------------------------------------JUMP MOVEMENT-------------------------------------------------------
class JumpUp final : public Command
{
public:
	JumpUp(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->Move(InputDirection::Up); // TODO : change the name move to detect or something , it doesn't move when this function is called for one input
		}
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->KeyReleased(InputDirection::Up);
		}
	};

	void Undo() override {};
};

class JumpDown final : public Command
{
public:
	JumpDown(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->Move(InputDirection::Down);
		}
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->KeyReleased(InputDirection::Down);
		}
	};
	void Undo() override {};
};

class JumpLeft final : public Command
{
public:
	JumpLeft(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->Move(InputDirection::Left);
		}
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->KeyReleased(InputDirection::Left);
		}
	};
	void Undo() override {};
};

class JumpRight final : public Command
{
public:
	JumpRight(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->Move(InputDirection::Right);
		}
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			//pPlayerActor->GetComponent<PlayerMovementComponent>()->KeyReleased(InputDirection::Right);
		}
	};
	void Undo() override {};
};

class ChooseSinglePlayerGameMode final : public Command
{
public:
	ChooseSinglePlayerGameMode(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		//GameStateManager::GetInstance().LoadGameMode(dae::Scene::GameMode::SinglePlayer);
	}
	void Release() const override
	{};
	void Undo() override {};
};

class ChooseCoOpGameMode final : public Command
{
public:
	ChooseCoOpGameMode(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		//GameStateManager::GetInstance().LoadGameMode(dae::Scene::GameMode::Coop);
	}
	void Release() const override
	{};
	void Undo() override {};
};

class ChooseVersusGameMode final : public Command
{
public:
	ChooseVersusGameMode(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		//GameStateManager::GetInstance().LoadGameMode(dae::Scene::GameMode::Versus);
	}
	void Release() const override
	{};
	void Undo() override {};
};

class GoToMenu final : public Command
{
public:
	GoToMenu(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		//GameStateManager::GetInstance().LoadMenuScreen();
	}
	void Release() const override
	{};
	void Undo() override {};
};