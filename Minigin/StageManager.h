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
		One = 0,
		Two = 1,
		Three = 2
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
	bool GetIsInMenu() const;
	void SetWindowSurface(SDL_Surface* windowSurface) { m_WindowSurface = windowSurface; };
	//
	void SetIsGameOver(bool isGameOver);
	void SetIsPlayerDead(bool isPlayerDead);
private:
	friend class dae::Singleton<StageManager>;
	StageManager() = default;
	//
	void LoadNextStage();
	//
	void LoadSinglePlayerMode();
	void LoadCoopMode();
	void LoadVersusMode();
	void DisplayResults(int score, int shotsFired, int numberOfHits, int accuracy);
	//
	SDL_Surface* m_WindowSurface = nullptr;
	Stage m_CurrentStage = Stage::One;
	GameMode m_CurrentGameMode = GameMode::SinglePlayer;
	//
	bool m_IsInMenu = true;
	bool m_IsGameOver = false;
	bool m_HasWon = false;
	bool m_IsPlayerDead = false;
	//
	float m_RestartTimer = 0.0f;
	const int m_RestartTime = 5;
	//
	float m_GameOverTimer = 0.0f;
	const int m_ActionTime = 4;
	float m_WinTimer = 0.0f;
};
