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

		if (m_GameOverTimer >= m_GameOverTime)
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
			player->SetIsActive(true);
			CollisionDetectionManager::GetInstance().AddCollisionGameObject(player);
			//
			m_RestartTimer -= m_RestartTimer;
			m_IsPlayerDead = false;
		}
	}
	//
	if (EnemyManager::GetInstance().GetAllEnemiesAreDead())
	{
		LoadNextStage();
	}
}

void StageManager::LoadNextStage()
{
	m_CurrentStage = Stage(int(m_CurrentStage) + 1);
	CollisionDetectionManager::GetInstance().ClearCollisions();
	FormationManager::GetInstance().InitFormation(m_CurrentStage);
	EnemyManager::GetInstance().DeleteAllEnemies();
	EnemyManager::GetInstance().IncreaseDifficulty();
	EnemyManager::GetInstance().SpawnAllEnemies(m_CurrentStage);
	//
	const int offset = 60;
	auto pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2 - offset * 2);
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	auto stageText = std::make_shared<GameObject>("STAGE " + std::to_string(int(m_CurrentStage) + 1));
	stageText->AddComponent(new TransformComponent(pos));
	stageText->AddComponent(new TextComponent("STAGE " + std::to_string(int(m_CurrentStage)), font, SDL_Color{ 255,0,0 }, false));
	stageText->GetComponent<TextComponent>()->SetIsVisible(true, 3);
	SceneManager::GetInstance().GetCurrentScene()->Add(stageText);
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
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec2(500, 780)));
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene->Add(scoreDisplay);
	//---------------------------------------------------------------------LIVES DISPLAY--------------------------------------------------
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec2(350, 780)));
	auto livesCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
	livesDisplay->AddComponent(livesCounter);
	scene->Add(livesDisplay);
	//---------------------------------------------------------------------ACCURACY DISPLAY--------------------------------------------------
	auto accuracyDisplay = std::make_shared<GameObject>("AccuracyDisplay");
	accuracyDisplay->AddComponent(new TransformComponent(glm::vec2(50, 780)));
	auto accuracyCounter = new TextComponent("Accuracy: 100 %", font, SDL_Color{ 255,255,255 });
	accuracyDisplay->AddComponent(accuracyCounter);
	scene->Add(accuracyDisplay);
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
	//player died text
	int offset = 50;
	auto pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2);
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	//
	auto readyText = std::make_shared<GameObject>("READY");
	readyText->AddComponent(new TransformComponent(pos));
	readyText->AddComponent(new TextComponent("READY", font, SDL_Color{ 255,0,0 }, false));
	readyText->GetComponent<TextComponent>()->SetIsVisible(true, 4.0f);
	scene->Add(readyText);
	//
	offset = 90;
	pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2);
	auto gameOver = std::make_shared<GameObject>("GAMEOVER");
	gameOver->AddComponent(new TransformComponent(pos));
	gameOver->AddComponent(new TextComponent("GAMEOVER", font, SDL_Color{ 255,0,0 }, false));
	scene->Add(gameOver);
	//
	offset = 60;
	pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2 - offset * 2);
	auto stageText = std::make_shared<GameObject>("STAGE " + std::to_string(int(m_CurrentStage) + 1));
	stageText->AddComponent(new TransformComponent(pos));
	stageText->AddComponent(new TextComponent("STAGE " + std::to_string(int(m_CurrentStage)), font, SDL_Color{ 255,0,0 }, false));
	stageText->GetComponent<TextComponent>()->SetIsVisible(true, 3);
	scene->Add(stageText);
	//-----------------------------------------------FIRST STAGE ENEMY QUEUES----------------------------------------
	FormationManager::GetInstance().InitFormation(m_CurrentStage);
	EnemyManager::GetInstance().SpawnAllEnemies(m_CurrentStage);
}

void StageManager::LoadCoopMode()
{
}

void StageManager::LoadVersusMode()
{
}

void StageManager::InitMenuScreen()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Galaga");
	//
	m_IsInMenu = true;
	m_IsGameOver = false;
	m_IsPlayerDead = false;
	m_RestartTimer = 0.0f;
	m_GameOverTimer = 0.0f;
	m_CurrentStage = Stage::One;
	//
	SceneManager::GetInstance().ClearScene(SceneManager::GetInstance().GetCurrentScene());
	CollisionDetectionManager::GetInstance().ClearCollisions();
	EnemyManager::GetInstance().DeleteAllEnemies();
	//
	SceneManager::GetInstance().SetScreenDimensions(glm::vec2{ m_WindowSurface->w,m_WindowSurface->h });

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	//gamemodes
	auto textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f)));
	TextComponent* text = new TextComponent("Controller : Press X for singleplayer, Y for Coop and B for Versus Mode. Press Select to QUIT.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 50)));
	text = new TextComponent("Keyboard : Press I for singleplayer, O for Coop and P for Versus Mode. Press ESCAPE to QUIT.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);

	//controls
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 150)));
	text = new TextComponent("Controller : D pad for movement , A to shoot (xbox controller).", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 200)));
	text = new TextComponent("Keyboard :A , D for movement, SPACE to shoot.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
}

void StageManager::InitWinScreen()
{
}

void StageManager::InitGameOverScreen()
{
	m_IsInMenu = false;
	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	//
	auto& scene = SceneManager::GetInstance().CreateScene("Galaga");

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//png
	auto losePng = std::make_shared<GameObject>("GameOverScreen");
	losePng->AddComponent(new TransformComponent(glm::vec2(0, 0)));
	losePng->AddComponent(new Texture2DComponent("GameOverScreen.png"));
	scene.Add(losePng);
	//
	auto textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 200, m_WindowSurface->h / 2.0f + 100)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto text = new TextComponent("Controller : Press START to go to main menu.", font, SDL_Color{ 255,0,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 200, m_WindowSurface->h / 2.0f + 150)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	text = new TextComponent("Keyboard : Press U to go to main menu.", font, SDL_Color{ 255,0,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
}

bool StageManager::GetIsInMenu() const
{
	return m_IsInMenu;
}

void StageManager::SetIsGameOver(bool isGameOver)
{
	m_IsGameOver = isGameOver;
}

void StageManager::SetIsPlayerDead(bool isPlayerDead)
{
	m_IsPlayerDead = isPlayerDead;
}