#pragma once
#include <iostream>
#include "../AliEngine/BaseCommand.h"
#include "../AliEngine/FPSTextComponent.h"
#include "ScoreComponent.h"
#include "../AliEngine/TextComponent.h"
#include "../AliEngine/Texture2DComponent.h"
#include "../AliEngine/TransformComponent.h"
#include "AnimationComponent.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/InputManager.h"
#include "GyaragaMovementComponent.h"
#include "PlayerWeaponComponent.h"
#include "StageManager.h"
#include "VersusControllerComponent.h"
#include "EnemyManager.h"

class ExitCommand final : public Command
{
public:
	ExitCommand(int index) :Command(index) {};

	void Execute() const override { dae::InputManager::GetInstance().SetHasExited(true); }
	void Release() const override {};
	void Undo() override {};
};

//-------------------------------------------------------------------------------------------------------PLAYER MOVEMENT-------------------------------------------------------
class SteerLeft final : public Command
{
public:
	SteerLeft(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor && pPlayerActor->GetIsActive())
		{
			pPlayerActor->GetComponent<GyaragaMovementComponent>()->Move(true);
		}
		else
		{
			Release();
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
		if (pPlayerActor && pPlayerActor->GetIsActive())
		{
			pPlayerActor->GetComponent<GyaragaMovementComponent>()->Move(false);
		}
		else
		{
			Release();
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

class Fire final : public Command
{
public:
	Fire(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayerActor && pPlayerActor->GetIsActive())
		{
			pPlayerActor->GetComponent<PlayerWeaponComponent>()->Shoot();
		}
	}

	void Release() const override {};

	void Undo() override {};
};
//-------------------------------------------------------------------------------------------------------VERSUS CONTROLS-------------------------------------------------------
class BossTractorBeam final : public Command
{
public:
	BossTractorBeam(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		if (m_ControllerIndex != 1)
		{
			return;
		}
		auto versusBoss = EnemyManager::GetInstance().GetAvailableBoss();

		if (versusBoss)
		{
			auto versusControlllerComp = versusBoss->GetComponent<VersusControllerComponent>();
			if (versusControlllerComp)
			{
				versusControlllerComp->SetIsTractorBeamActivated(true);
			}
		}
	}

	void Release() const override {};

	void Undo() override {};
};

class BossDive final : public Command
{
public:
	BossDive(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		if (m_ControllerIndex != 1)
		{
			return;
		}

		auto versusBoss = EnemyManager::GetInstance().GetAvailableBoss();

		if (versusBoss)
		{
			auto versusControlllerComp = versusBoss->GetComponent<VersusControllerComponent>();
			if (versusControlllerComp)
			{
				versusControlllerComp->SetIsDivingActivated(true);
			}
		}
	}

	void Release() const override {};

	void Undo() override {};
};

class BossShoot final : public Command
{
public:
	BossShoot(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		if (m_ControllerIndex != 1)
		{
			return;
		}
		auto versusBoss = EnemyManager::GetInstance().GetAvailableBoss();

		if (versusBoss)
		{
			auto versusControlllerComp = versusBoss->GetComponent<VersusControllerComponent>();
			if (versusControlllerComp)
			{
				versusControlllerComp->SetIsShootingActivated(true);
			}
		}
	}

	void Release() const override {};

	void Undo() override {};
};
//-------------------------------------------------------------------------------------------------------------GAMEMODES AND MENU--------------------------------------------------------
class ChooseSinglePlayerGameMode final : public Command
{
public:
	ChooseSinglePlayerGameMode(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		StageManager::GetInstance().LoadGameMode(StageManager::GameMode::SinglePlayer);
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
		StageManager::GetInstance().LoadGameMode(StageManager::GameMode::Coop);
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
		StageManager::GetInstance().LoadGameMode(StageManager::GameMode::Versus);
	}
	void Release() const override
	{};
	void Undo() override {};
};

class GoToStartScreen final : public Command
{
public:
	GoToStartScreen(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		StageManager::GetInstance().InitMenuScreen();
	}
	void Release() const override
	{};
	void Undo() override {};
};