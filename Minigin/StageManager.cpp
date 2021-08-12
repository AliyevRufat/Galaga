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

using namespace dae;

void StageManager::Update()
{
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();

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
	const int offset = 40;
	auto pos = glm::vec2(m_WindowSurface->w / 2 - offset, m_WindowSurface->h / 2);
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);
	//
	auto readyText = std::make_shared<GameObject>("READY");
	readyText->AddComponent(new TransformComponent(pos));
	readyText->AddComponent(new TextComponent("READY", font, SDL_Color{ 255,0,0 }, false));
	scene->Add(readyText);
	//
	auto gameOver = std::make_shared<GameObject>("GAMEOVER");
	gameOver->AddComponent(new TransformComponent(pos));
	gameOver->AddComponent(new TextComponent("GAMEOVER", font, SDL_Color{ 255,0,0 }, false));
	scene->Add(gameOver);
	//-----------------------------------------------FIRST STAGE ENEMY QUEUES----------------------------------------
	//{
	//	{
	//		//first wave
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 4);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 4);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 5);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 5);
	//		//
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 3, true);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 3, true);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 4, true);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 4, true);
	//
	//		EnemyManager::GetInstance().Wait();
	//	}
	//
	//	{
	//		//second wave
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 0, 0);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 2);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 1, 1);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 2);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 0, 2);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 5);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 1, 3);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 5);
	//		//
	//		EnemyManager::GetInstance().Wait();
	//	}
	//
	//	{
	//		//third wave
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 0);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 0);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 1);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 1);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 6);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 6);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 7);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 7);
	//		//
	//		EnemyManager::GetInstance().Wait();
	//	}
	//
	//	{
	//		//forth wave
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 2);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 2);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 3);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 3);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 6);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 6);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 7);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 7);
	//		//
	//		EnemyManager::GetInstance().Wait();
	//	}
	//
	//	{
	//		//five wave
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 0);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 0);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 1);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 1);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 8);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 8);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 9);
	//		EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 9);
	//		//
	//		EnemyManager::GetInstance().Wait();
	//	}
	//}
	//-----------------------------------------------SECOND STAGE ENEMY QUEUES----------------------------------------
	{
		{
			//first wave
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 4);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 4);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 4);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 5);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 5);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 5);

			EnemyManager::GetInstance().Wait();
		}

		{
			//second wave
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 0, 0);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 0, 1);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 0);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 1);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 2);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 0, 3);
			//
			EnemyManager::GetInstance().Wait();
		}

		{
			//third wave
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 1, 0);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Boss, 1, 1);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 0);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 1);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 2);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Butterfly, 1, 3);
			//
			EnemyManager::GetInstance().Wait();
		}

		{
			//forth wave
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 2);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 2);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 2);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 3);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 3);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 3);

			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 6);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 6);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 6);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 7);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 7);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 7);
			//
			EnemyManager::GetInstance().Wait();
		}

		{
			//five wave
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 0);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 0);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 0);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 1);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 1);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 1);

			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 8);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 8);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 8);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 0, 9);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 1, 9);
			EnemyManager::GetInstance().QueueEnemy(EnemyType::Bee, 2, 9);
			//
			EnemyManager::GetInstance().Wait();
		}
	}
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
	losePng->AddComponent(new TransformComponent(glm::vec2(-350, -200)));
	losePng->AddComponent(new Texture2DComponent("GameOverScreen.png"));
	scene.Add(losePng);
	//
	auto textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 100)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto text = new TextComponent("Controller : Press START to go to main menu.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 150)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	text = new TextComponent("Keyboard : Press U to go to main menu.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
}

StageManager::Stage StageManager::GetCurrentStage() const
{
	return m_CurrentStage;
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