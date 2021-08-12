#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "ComponentIncludes.h"
#include <vector>

class StageManager final : public dae::Singleton<StageManager>
{
public:

	enum class GameMode
	{
		SinglePlayer,
		Coop,
		Versus
	};

	enum class Stage
	{
		One,
		Two,
		Three
	};

	void Update();
	//
	void LoadGameMode(GameMode gameMode);
	//
	void InitStageOne();
	void InitStageTwo();
	void InitStageThree();
	//
	void InitMenuScreen();
	void InitWinScreen();
	void InitGameOverScreen();
	//
	Stage GetCurrentStage() const;
	bool GetIsInMenu() const;
	void SetWindowSurface(SDL_Surface* windowSurface) { m_WindowSurface = windowSurface; };
	//
	void SetIsGameOver(bool isGameOver);
	void SetIsPlayerDead(bool isPlayerDead);
private:
	friend class dae::Singleton<StageManager>;
	StageManager() = default;
	//
	void LoadSinglePlayerMode();
	void LoadCoopMode();
	void LoadVersusMode();
	//
	SDL_Surface* m_WindowSurface = nullptr;
	Stage m_CurrentStage = Stage::One;
	GameMode m_CurrentGameMode = GameMode::SinglePlayer;
	//
	bool m_IsInMenu = true;
	bool m_IsGameOver = false;
	bool m_IsPlayerDead = false;
	//
	float m_RestartTimer = 0.0f;
	const int m_RestartTime = 5;
	//
	float m_GameOverTimer = 0.0f;
	const int m_GameOverTime = 4;
};
