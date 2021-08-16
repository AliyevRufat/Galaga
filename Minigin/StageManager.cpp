#include "MiniginPCH.h"
#include "StageManager.h"
#include "EnemyManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CollisionDetectionManager.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ComponentIncludes.h"
#include "LivesObserver.h"
#include "ScoreObserver.h"
#include "AccuracyObserver.h"
#include "ScoreComponent.h"
#include "Texture2DComponent.h"
#include "FPSTextComponent.h"
#include "PlayerWeaponComponent.h"
#include "GyaragaMovementComponent.h"
#include "FormationManager.h"

using namespace dae;

void StageManager::Update()
{
	//updating formation movement, enemies and collisions
	FormationManager::GetInstance().Update();
	EnemyManager::GetInstance().Update();
	CollisionDetectionManager::GetInstance().Update();
	//
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();
	//restart player if dead or show game over screen of lost
	if (m_IsGameOver)
	{
		m_GameOverTimer += deltaTime;

		if (m_GameOverTimer >= m_ActionTime)
		{
			InitGameOverScreen();
			//
			m_GameOverTimer -= m_GameOverTimer;
			m_IsGameOver = false;
		}
	}
	//
	if (m_IsPlayerDead)
	{
		m_RestartTimer += deltaTime;

		if (m_RestartTimer >= m_RestartTime)
		{
			auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);

			if (player)
			{
				if (!player->GetIsActive())
				{
					player->SetIsActive(true);
					CollisionDetectionManager::GetInstance().AddCollisionGameObject(player);
				}
			}
			//
			m_RestartTimer -= m_RestartTimer;
			m_IsPlayerDead = false;
		}
	}
	if (m_IsPlayer2Dead)
	{
		m_RestartTimerPlayer2 += deltaTime;

		if (m_RestartTimerPlayer2 >= m_RestartTime)
		{
			auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);

			if (player2)
			{
				if (!player2->GetIsActive())
				{
					player2->SetIsActive(true);
					CollisionDetectionManager::GetInstance().AddCollisionGameObject(player2);
				}
			}
			//
			m_RestartTimerPlayer2 -= m_RestartTimerPlayer2;
			m_IsPlayer2Dead = false;
		}
	}
	//
	if (m_HasWon && !m_IsGameOver)
	{
		m_WinTimer += deltaTime;

		if (m_WinTimer >= m_ActionTime)
		{
			InitWinScreen();
			//
			m_WinTimer -= m_WinTimer;
			m_HasWon = false;
		}
	}
	//
	if (EnemyManager::GetInstance().GetAllEnemiesAreDead() && !m_IsGameOver)
	{
		LoadNextStage();
	}
}

void StageManager::LoadNextStage()
{
	if (m_CurrentStage == Stage::Three)
	{
		m_HasWon = true;
		return;
	}
	//
	m_CurrentStage = Stage(int(m_CurrentStage) + 1);
	CollisionDetectionManager::GetInstance().ClearCollisions();
	FormationManager::GetInstance().InitFormation(m_CurrentStage);
	EnemyManager::GetInstance().DeleteAllEnemies();
	EnemyManager::GetInstance().IncreaseDifficulty();
	EnemyManager::GetInstance().SpawnAllEnemies(m_CurrentStage);
	//
	const int offset = 60;
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	auto go = DisplayText("STAGE " + std::to_string(int(m_CurrentStage) + 1), font, "STAGE " + std::to_string(int(m_CurrentStage) + 1), glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2 - offset * 2), glm::vec3(255, 0, 0), false);
	go->GetComponent<TextComponent>()->SetIsVisible(true, 3);
}

void StageManager::LoadGameMode(GameMode gameMode)
{
	if (!m_IsInMenu)
	{
		return;
	}
	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	m_CurrentGameMode = gameMode;

	switch (gameMode)
	{
	case GameMode::SinglePlayer:
		LoadSinglePlayerMode();
		break;
	case GameMode::Coop:
		LoadCoopMode();
		break;
	case GameMode::Versus:
		LoadVersusMode();
		break;
	}
	m_IsInMenu = false;
}

void StageManager::InitStageOne()
{
}

void StageManager::InitStageTwo()
{
}

void StageManager::InitStageThree()
{
}

void StageManager::LoadSinglePlayerMode()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	//---------------------------------------------------------------------FPS COUNTER--------------------------------------------------
	auto go = std::make_shared<GameObject>("FPSCounter");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font));
	scene->Add(go);
	//---------------------------------------------------------------------SCORE DISPLAY--------------------------------------------------
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	DisplayText("ScoreDisplay", font, "Score: 0", glm::vec2(500, 780), glm::vec3(255, 255, 255));
	//---------------------------------------------------------------------LIVES DISPLAY--------------------------------------------------
	DisplayText("LivesDisplay", font, "Lives: 2", glm::vec2(350, 780), glm::vec3(255, 255, 255));
	//---------------------------------------------------------------------ACCURACY DISPLAY--------------------------------------------------
	DisplayText("AccuracyDisplay", font, "Accuracy: 100 %", glm::vec2(50, 780), glm::vec3(255, 255, 255));
	//---------------------------------------------------------------------PLAYER--------------------------------------------------
	const int playerScale = 1;
	const int playerWidth = 45 * playerScale;
	const int playerHeight = 43 * playerScale;
	auto gyaraga = std::make_shared<GameObject>("Gyaraga", nullptr, glm::vec2(playerWidth, playerHeight));
	//Components
	gyaraga->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2 - playerWidth / 2, m_WindowSurface->h - m_WindowSurface->h / 8 - playerHeight / 2), glm::vec2(playerWidth, playerHeight)));
	gyaraga->AddComponent(new HealthComponent(2));
	gyaraga->AddComponent(new ScoreComponent(0));
	gyaraga->AddComponent(new Texture2DComponent("Gyaraga.png", playerScale, false));
	gyaraga->AddComponent(new GyaragaMovementComponent());
	gyaraga->AddComponent(new PlayerWeaponComponent());
	//watchers
	gyaraga->AddWatcher(new ScoreObserver());
	gyaraga->AddWatcher(new LivesObserver());
	gyaraga->AddWatcher(new AccuracyObserver());
	//Adds
	scene->Add(gyaraga);
	scene->AddPlayer(gyaraga);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(gyaraga);
	SceneManager::GetInstance().GetCurrentScene()->AddPlayer(gyaraga);
	//Texts
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	//ready text
	int offset = 50;
	auto pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2);
	go = DisplayText("READY", font, "READY", pos, glm::vec3(255, 0, 0), false);
	go->GetComponent<TextComponent>()->SetIsVisible(true, 4.0f);
	//gameover text
	offset = 90;
	pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2);
	DisplayText("GAMEOVER", font, "GAMEOVER", pos, glm::vec3(255, 0, 0), false);
	//
	offset = 60;
	pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2 - offset * 2);
	go = DisplayText("STAGE " + std::to_string(int(m_CurrentStage) + 1), font, "STAGE " + std::to_string(int(m_CurrentStage) + 1), pos, glm::vec3(255, 0, 0));
	go->GetComponent<TextComponent>()->SetIsVisible(true, 3);
	//-----------------------------------------------FIRST STAGE ENEMY QUEUES----------------------------------------
	FormationManager::GetInstance().InitFormation(m_CurrentStage);
	EnemyManager::GetInstance().SpawnAllEnemies(m_CurrentStage);
}

void StageManager::LoadCoopMode()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	//---------------------------------------------------------------------FPS COUNTER--------------------------------------------------
	auto go = std::make_shared<GameObject>("FPSCounter");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font));
	scene->Add(go);
	//---------------------------------------------------------------------SCORE DISPLAY--------------------------------------------------
	//player 1
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	DisplayText("ScoreDisplay", font, "Score P1 : 0", glm::vec2(500, 780), glm::vec3(255, 255, 255));
	//player 2
	DisplayText("ScoreDisplay2", font, "Score P2 : 0", glm::vec2(500, 820), glm::vec3(255, 255, 255));
	//---------------------------------------------------------------------LIVES DISPLAY--------------------------------------------------
	//player 1
	DisplayText("LivesDisplay", font, "Lives P1 : 2", glm::vec2(350, 780), glm::vec3(255, 255, 255));
	//player 2
	DisplayText("LivesDisplay2", font, "Lives P2 : 2", glm::vec2(350, 780), glm::vec3(255, 255, 255));
	//---------------------------------------------------------------------ACCURACY DISPLAY--------------------------------------------------
	//player 1
	DisplayText("AccuracyDisplay", font, "Accuracy P1 : 100 %", glm::vec2(50, 780), glm::vec3(255, 255, 255));
	//player 2
	DisplayText("AccuracyDisplay2", font, "Accuracy P2 : 100 %", glm::vec2(50, 820), glm::vec3(255, 255, 255));
	//---------------------------------------------------------------------PLAYER1--------------------------------------------------
	const int spawnOffset = 50;
	const int playerScale = 1;
	const int playerWidth = 45 * playerScale;
	const int playerHeight = 43 * playerScale;
	auto gyaraga = std::make_shared<GameObject>("Gyaraga", nullptr, glm::vec2(playerWidth, playerHeight));
	//Components
	gyaraga->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2 - playerWidth / 2 - spawnOffset, m_WindowSurface->h - m_WindowSurface->h / 8 - playerHeight / 2), glm::vec2(playerWidth, playerHeight)));
	gyaraga->AddComponent(new HealthComponent(2));
	gyaraga->AddComponent(new ScoreComponent(0));
	gyaraga->AddComponent(new Texture2DComponent("Gyaraga.png", playerScale, false));
	gyaraga->AddComponent(new GyaragaMovementComponent());
	gyaraga->AddComponent(new PlayerWeaponComponent());
	//watchers
	gyaraga->AddWatcher(new ScoreObserver());
	gyaraga->AddWatcher(new LivesObserver());
	gyaraga->AddWatcher(new AccuracyObserver());
	//Adds
	scene->Add(gyaraga);
	scene->AddPlayer(gyaraga);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(gyaraga);
	//---------------------------------------------------------------------PLAYER2--------------------------------------------------
	auto gyaraga2 = std::make_shared<GameObject>("Gyaraga2", nullptr, glm::vec2(playerWidth, playerHeight));
	//Components
	gyaraga2->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2 - playerWidth / 2 + spawnOffset, m_WindowSurface->h - m_WindowSurface->h / 8 - playerHeight / 2), glm::vec2(playerWidth, playerHeight)));
	gyaraga2->AddComponent(new HealthComponent(2));
	gyaraga2->AddComponent(new ScoreComponent(0));
	gyaraga2->AddComponent(new Texture2DComponent("Gyaraga2.png", playerScale, false));
	gyaraga2->AddComponent(new GyaragaMovementComponent());
	gyaraga2->AddComponent(new PlayerWeaponComponent());
	//watchers
	gyaraga2->AddWatcher(new ScoreObserver());
	gyaraga2->AddWatcher(new LivesObserver());
	gyaraga2->AddWatcher(new AccuracyObserver());
	//Adds
	scene->Add(gyaraga2);
	scene->AddPlayer(gyaraga2);
	CollisionDetectionManager::GetInstance().AddCollisionGameObject(gyaraga2);
	//---------------------------------------------------------------------POP UP TEXTS-----------------------------------------------------
	int offset = 90;
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	auto pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2);
	//gameover text
	DisplayText("GAMEOVER", font, "GAMEOVER", pos, glm::vec3(255, 0, 0), false);
	//stage number text
	offset = 60;
	pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2 - offset * 2);
	go = DisplayText("STAGE " + std::to_string(int(m_CurrentStage) + 1), font, "STAGE " + std::to_string(int(m_CurrentStage) + 1), pos, glm::vec3(255, 0, 0), false);
	go->GetComponent<TextComponent>()->SetIsVisible(true, 3);
	//-----------------------------------------------FIRST STAGE ENEMY QUEUES----------------------------------------
	FormationManager::GetInstance().InitFormation(m_CurrentStage);
	EnemyManager::GetInstance().SpawnAllEnemies(m_CurrentStage);
}

void StageManager::LoadVersusMode()
{
}

void StageManager::InitMenuScreen()
{
	SceneManager::GetInstance().CreateScene("Galaga");
	//
	m_HasWon = false;
	m_IsInMenu = true;
	m_IsGameOver = false;
	m_IsPlayerDead = false;
	m_IsPlayer2Dead = false;
	m_WinTimer = 0.0f;
	m_RestartTimer = 0.0f;
	m_RestartTimerPlayer2 = 0.0f;
	m_GameOverTimer = 0.0f;
	m_CoopPlayerDeathCount = 0;
	m_CurrentStage = Stage::One;
	//
	SceneManager::GetInstance().ClearScene(SceneManager::GetInstance().GetCurrentScene());
	CollisionDetectionManager::GetInstance().ClearCollisions();
	EnemyManager::GetInstance().DeleteAllEnemies();
	//
	SceneManager::GetInstance().SetScreenDimensions(glm::vec2{ m_WindowSurface->w,m_WindowSurface->h });

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	//info text
	DisplayText("Text", font, "Controller : Press X for singleplayer, Y for Coop and B for Versus Mode. Press Select to QUIT.", glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f), glm::vec3(255, 255, 0));
	DisplayText("Text", font, "Keyboard : Press I for singleplayer, O for Coop and P for Versus Mode. Press ESCAPE to QUIT.", glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 50), glm::vec3(255, 255, 0));
	DisplayText("Text", font, "Controller : D pad for movement , A to shoot (xbox controller).", glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 150), glm::vec3(255, 255, 0));
	DisplayText("Text", font, "Keyboard :A , D for movement, SPACE to shoot.", glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 200), glm::vec3(255, 255, 0));
}

void StageManager::InitWinScreen()
{
	m_IsInMenu = false;

	auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);
	auto playerWeaponComp = player->GetComponent<PlayerWeaponComponent>();
	//
	const int score = player->GetComponent<ScoreComponent>()->GetScore();
	const int shotsFired = playerWeaponComp->GetAmountOfShotBullets();
	const int numberOfHits = playerWeaponComp->GetAmountOfHits();
	const int accuracy = playerWeaponComp->GetAccuracy();
	//
	auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);
	int scoreP2 = 0, shotsFiredP2 = 0, numberOfHitsP2 = 0, accuracyP2 = 0;
	if (player2)
	{
		auto playerWeaponComp2 = player2->GetComponent<PlayerWeaponComponent>();
		//
		scoreP2 = player2->GetComponent<ScoreComponent>()->GetScore();
		shotsFiredP2 = playerWeaponComp2->GetAmountOfShotBullets();
		numberOfHitsP2 = playerWeaponComp2->GetAmountOfHits();
		accuracyP2 = playerWeaponComp2->GetAccuracy();
	}

	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	//
	auto& scene = SceneManager::GetInstance().CreateScene("Qbert");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//png
	auto winPng = std::make_shared<GameObject>("WinScreen");
	winPng->AddComponent(new TransformComponent(glm::vec2(0, -200)));
	winPng->AddComponent(new Texture2DComponent("WinScreen.png"));
	scene.Add(winPng);

	//info text
	DisplayText("Text", font, "Controller : Press Y to go to main menu.", glm::vec2(m_WindowSurface->w / 2.0f - 170, m_WindowSurface->h / 2.0f + 250), glm::vec3(255, 255, 0));
	DisplayText("Text", font, "Keyboard : Press U to go to main menu.", glm::vec2(m_WindowSurface->w / 2.0f - 170, m_WindowSurface->h / 2.0f + 300), glm::vec3(255, 255, 0));
	//
	if (m_CurrentGameMode == GameMode::Coop)
	{
		DisplayResults(score, shotsFired, numberOfHits, accuracy, scoreP2, shotsFiredP2, numberOfHitsP2, accuracyP2);
	}
	else
	{
		DisplayResults(score, shotsFired, numberOfHits, accuracy);
	}
}

void StageManager::InitGameOverScreen()
{
	m_IsInMenu = false;

	auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);
	auto playerWeaponComp = player->GetComponent<PlayerWeaponComponent>();
	//
	const int score = player->GetComponent<ScoreComponent>()->GetScore();
	const int shotsFired = playerWeaponComp->GetAmountOfShotBullets();
	const int numberOfHits = playerWeaponComp->GetAmountOfHits();
	const int accuracy = playerWeaponComp->GetAccuracy();

	auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);
	int scoreP2 = 0, shotsFiredP2 = 0, numberOfHitsP2 = 0, accuracyP2 = 0;
	if (player2)
	{
		auto playerWeaponComp2 = player2->GetComponent<PlayerWeaponComponent>();
		//
		scoreP2 = player2->GetComponent<ScoreComponent>()->GetScore();
		shotsFiredP2 = playerWeaponComp2->GetAmountOfShotBullets();
		numberOfHitsP2 = playerWeaponComp2->GetAmountOfHits();
		accuracyP2 = playerWeaponComp2->GetAccuracy();
	}

	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	//
	auto& scene = SceneManager::GetInstance().CreateScene("Galaga");

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	//png
	auto losePng = std::make_shared<GameObject>("GameOverScreen");
	losePng->AddComponent(new TransformComponent(glm::vec2(0, -200)));
	losePng->AddComponent(new Texture2DComponent("GameOverScreen.png"));
	scene.Add(losePng);
	//info text
	DisplayText("Text", font, "Controller : Press START to go to main menu.", glm::vec2(m_WindowSurface->w / 2.0f - 200, m_WindowSurface->h / 2.0f + 100), glm::vec3(255, 0, 0));
	DisplayText("Text", font, "Keyboard : Press U to go to main menu.", glm::vec2(m_WindowSurface->w / 2.0f - 200, m_WindowSurface->h / 2.0f + 150), glm::vec3(255, 0, 0));
	//
	if (m_CurrentGameMode == GameMode::Coop)
	{
		DisplayResults(score, shotsFired, numberOfHits, accuracy, scoreP2, shotsFiredP2, numberOfHitsP2, accuracyP2);
	}
	else
	{
		DisplayResults(score, shotsFired, numberOfHits, accuracy);
	}
}

bool StageManager::GetIsInMenu() const
{
	return m_IsInMenu;
}

void StageManager::SetIsGameOver(bool isGameOver)
{
	if (m_CurrentGameMode == GameMode::Coop)
	{
		++m_CoopPlayerDeathCount;
		if (m_CoopPlayerDeathCount >= 2)
		{
			m_IsGameOver = isGameOver;
		}
	}
	else
	{
		m_IsGameOver = isGameOver;
	}
}

void StageManager::SetIsPlayerDead(bool isPlayerDead, const std::string& playerName)
{
	if (playerName == "Gyaraga2")
	{
		m_IsPlayer2Dead = isPlayerDead;
	}
	else
	{
		m_IsPlayerDead = isPlayerDead;
	}
}

void StageManager::DisplayResults(int score, int shotsFired, int numberOfHits, int accuracy, int scoreP2, int shotsFiredP2, int numberOfHitsP2, int accuracyP2)
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	if (m_CurrentGameMode == GameMode::Coop)
	{
		const int posOffsetP1 = 300;
		const int posOffsetP2 = 100;
		//results text
		DisplayText("Results", font, "-RESULTS-", glm::vec2(m_WindowSurface->w / 2.0f - 100, m_WindowSurface->h / 2.0f - 150), glm::vec3(255, 0, 0));
		//display score
		DisplayText("Score", font, "SCORE : " + std::to_string(score), glm::vec2(m_WindowSurface->w / 2.0f - posOffsetP1, m_WindowSurface->h / 2.0f - 100), glm::vec3(255, 255, 0));
		//fired shots
		DisplayText("FiredShots", font, "SHOTS FIRED : " + std::to_string(shotsFired), glm::vec2(m_WindowSurface->w / 2.0f - posOffsetP1, m_WindowSurface->h / 2.0f - 50), glm::vec3(255, 255, 0));
		//number of hits
		DisplayText("NumberOfHits", font, "NUMBER OF HITS : " + std::to_string(numberOfHits), glm::vec2(m_WindowSurface->w / 2.0f - posOffsetP1, m_WindowSurface->h / 2.0f), glm::vec3(255, 255, 0));
		//accuracy
		DisplayText("HitMissRatio", font, "HIT MISS RATIO	 : %" + std::to_string(accuracy), glm::vec2(m_WindowSurface->w / 2.0f - posOffsetP1, m_WindowSurface->h / 2.0f + 50), glm::vec3(255, 255, 255));
		//display score p2
		DisplayText("Score", font, "SCORE : " + std::to_string(scoreP2), glm::vec2(m_WindowSurface->w / 2.0f + posOffsetP2, m_WindowSurface->h / 2.0f - 100), glm::vec3(255, 255, 0));
		//fired shots p2
		DisplayText("FiredShots", font, "SHOTS FIRED : " + std::to_string(shotsFiredP2), glm::vec2(m_WindowSurface->w / 2.0f + posOffsetP2, m_WindowSurface->h / 2.0f - 50), glm::vec3(255, 255, 0));
		//number of hits p2
		DisplayText("NumberOfHits", font, "NUMBER OF HITS : " + std::to_string(numberOfHitsP2), glm::vec2(m_WindowSurface->w / 2.0f + posOffsetP2, m_WindowSurface->h / 2.0f), glm::vec3(255, 255, 0));
		//accuracy p2
		DisplayText("HitMissRatio", font, "HIT MISS RATIO	 : %" + std::to_string(accuracyP2), glm::vec2(m_WindowSurface->w / 2.0f + posOffsetP2, m_WindowSurface->h / 2.0f + 50), glm::vec3(255, 255, 255));
	}
	else
	{
		//results text
		DisplayText("Results", font, "-RESULTS-", glm::vec2(m_WindowSurface->w / 2.0f - 20, m_WindowSurface->h / 2.0f - 150), glm::vec3(255, 0, 0));
		//display score
		DisplayText("Score", font, "SCORE : " + std::to_string(score), glm::vec2(m_WindowSurface->w / 2.0f - 80, m_WindowSurface->h / 2.0f - 100), glm::vec3(255, 255, 0));
		//fired shots
		DisplayText("FiredShots", font, "SHOTS FIRED : " + std::to_string(shotsFired), glm::vec2(m_WindowSurface->w / 2.0f - 80, m_WindowSurface->h / 2.0f - 50), glm::vec3(255, 255, 0));
		//number of hits
		DisplayText("NumberOfHits", font, "NUMBER OF HITS : " + std::to_string(numberOfHits), glm::vec2(m_WindowSurface->w / 2.0f - 80, m_WindowSurface->h / 2.0f), glm::vec3(255, 255, 0));
		//accuracy
		DisplayText("HitMissRatio", font, "HIT MISS RATIO	 : %" + std::to_string(accuracy), glm::vec2(m_WindowSurface->w / 2.0f - 80, m_WindowSurface->h / 2.0f + 50), glm::vec3(255, 255, 255));
	}
}

StageManager::GameMode StageManager::GetCurrentGameMode() const
{
	return m_CurrentGameMode;
}

std::shared_ptr<GameObject> StageManager::DisplayText(const std::string& name, const std::shared_ptr<dae::Font>& font, const std::string& text, const glm::vec2& pos, const glm::vec3& color, bool isVisible)
{
	auto display = std::make_shared<GameObject>(name);
	display->AddComponent(new TransformComponent(pos));
	auto displayText = new TextComponent(text, font, SDL_Color{ Uint8(color.x),Uint8(color.y),Uint8(color.z) }, isVisible);
	display->AddComponent(displayText);
	dae::SceneManager::GetInstance().GetCurrentScene()->Add(display);
	return display;
}