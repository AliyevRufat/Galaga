#pragma once
#include <iostream>
#include "BaseCommand.h"
#include "HealthComponent.h"
#include "FPSTextComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "GyaragaMovementComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"

class ExitCommand final : public Command
{
public:
	ExitCommand(int index) :Command(index) {};

	void Execute() const override { dae::InputManager::GetInstance().SetHasExited(true); }
	void Release() const override {};
	void Undo() override {};
};

//-------------------------------------------------------------------------------------------------------JUMP MOVEMENT-------------------------------------------------------
class SteerLeft final : public Command
{
public:
	SteerLeft(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			pPlayerActor->GetComponent<GyaragaMovementComponent>()->Move(true);
		}
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			pPlayerActor->GetComponent<GyaragaMovementComponent>()->StopMoving(true);
		}
	};

	void Undo() override {};
};

class SteerRight final : public Command
{
public:
	SteerRight(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			pPlayerActor->GetComponent<GyaragaMovementComponent>()->Move(false);
		}
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor)
		{
			pPlayerActor->GetComponent<GyaragaMovementComponent>()->StopMoving(false);
		}
	};
	void Undo() override {};
};
//
//class ChooseSinglePlayerGameMode final : public Command
//{
//public:
//	ChooseSinglePlayerGameMode(int index) :Command(index) { m_ControllerIndex = index; };
//
//	void Execute() const override
//	{
//		//GameStateManager::GetInstance().LoadGameMode(dae::Scene::GameMode::SinglePlayer);
//	}
//	void Release() const override
//	{};
//	void Undo() override {};
//};
//
//class ChooseCoOpGameMode final : public Command
//{
//public:
//	ChooseCoOpGameMode(int index) :Command(index) { m_ControllerIndex = index; };
//
//	void Execute() const override
//	{
//		//GameStateManager::GetInstance().LoadGameMode(dae::Scene::GameMode::Coop);
//	}
//	void Release() const override
//	{};
//	void Undo() override {};
//};
//
//class ChooseVersusGameMode final : public Command
//{
//public:
//	ChooseVersusGameMode(int index) :Command(index) { m_ControllerIndex = index; };
//
//	void Execute() const override
//	{
//		//GameStateManager::GetInstance().LoadGameMode(dae::Scene::GameMode::Versus);
//	}
//	void Release() const override
//	{};
//	void Undo() override {};
//};
//
//class GoToMenu final : public Command
//{
//public:
//	GoToMenu(int index) :Command(index) { m_ControllerIndex = index; };
//
//	void Execute() const override
//	{
//		//GameStateManager::GetInstance().LoadMenuScreen();
//	}
//	void Release() const override
//	{};
//	void Undo() override {};
//};