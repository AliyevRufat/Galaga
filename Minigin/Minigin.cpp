#include "MiniginPCH.h"
#include "Minigin.h"
#include "SDL.h"
#include <chrono>
#include <thread>
#include "Commands.h"
#include "../AliEngine/Observer.h"
#include "../AliEngine/ConsoleAudioService.h"
#include "../AliEngine/Locator.h"
#include "../AliEngine/Renderer.h"
#include "../AliEngine/ResourceManager.h"
#include "../AliEngine/InputManager.h"
#include "../AliEngine/EngineTime.h"
#include "LivesObserver.h"
#include "ScoreObserver.h"
#include "GyaragaMovementComponent.h"
#include "PlayerWeaponComponent.h"
#include "CollisionDetectionManager.h"
#include "EnemyManager.h"
#include "AccuracyObserver.h"
#include "StageManager.h"
#include "FormationManager.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init_Video Error: ") + SDL_GetError());
	}

	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init_Audio Error: ") + SDL_GetError());
	}

	int frequency = 44100;
	int chunkSize = 2048;
	int channels = 2;

	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) < 0)
		throw std::runtime_error(std::string("SDL_Audio Error: ") + Mix_GetError());

	m_Window = SDL_CreateWindow(
		"GALAGA",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		700,
		850,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	Renderer::GetInstance().Init(m_Window);
	Locator::Provide(new ConsoleAudioService());
	Locator::GetAudio().AddSound("../Data/Sounds/S_ThemeSong.mp3", false);
	Locator::GetAudio().AddSound("../Data/Sounds/S_EnemyDeath.wav", true);
	Locator::GetAudio().AddSound("../Data/Sounds/S_EnemyDeath2.wav", true);
	Locator::GetAudio().AddSound("../Data/Sounds/S_FighterCaptured.wav", true);
	Locator::GetAudio().AddSound("../Data/Sounds/S_Fire.wav", true);
	Locator::GetAudio().AddSound("../Data/Sounds/S_LevelStart.wav", true);
	Locator::GetAudio().AddSound("../Data/Sounds/S_Win.wav", true);
	Locator::GetAudio().AddSound("../Data/Sounds/S_PlayerDeath.wav", true);
	Locator::GetAudio().AddSound("../Data/Sounds/S_TractorBeam.wav", true);
	FormationManager::GetInstance().InitFormation();
}

void dae::Minigin::BindCommands()
{
	auto& inputManager = InputManager::GetInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		//assign buttons
		inputManager.AssignKey<ChooseSinglePlayerGameMode>(ControllerButton::ButtonX, i);
		inputManager.AssignKey<ChooseCoOpGameMode>(ControllerButton::ButtonY, i);
		inputManager.AssignKey<ChooseVersusGameMode>(ControllerButton::ButtonB, i);
		inputManager.AssignKey<GoToStartScreen>(ControllerButton::ButtonStart, i);
		inputManager.AssignKey<ExitCommand>(ControllerButton::ButtonSelect, i);
		//gameplay
		inputManager.AssignKey<SteerLeft>(ControllerButton::ButtonLeft, i);
		inputManager.AssignKey<SteerRight>(ControllerButton::ButtonRight, i);
		inputManager.AssignKey<Fire>(ControllerButton::ButtonA, i);
		//versus controller
		inputManager.AssignKey<BossTractorBeam>(ControllerButton::ButtonRightShoulder, i);
		inputManager.AssignKey<BossDive>(ControllerButton::ButtonLeftShoulder, i);
		inputManager.AssignKey<BossShoot>(ControllerButton::ButtonUp, i);
	}

	//keyboard

	//menu
	inputManager.AssignKey<ChooseSinglePlayerGameMode>(KeyboardButton::I, 0);
	inputManager.AssignKey<ChooseCoOpGameMode>(KeyboardButton::O, 0);
	inputManager.AssignKey<ChooseVersusGameMode>(KeyboardButton::P, 0);
	inputManager.AssignKey<GoToStartScreen>(KeyboardButton::U, 0);
	inputManager.AssignKey<ExitCommand>(KeyboardButton::ESCAPE, 0);
	//gameplay
	inputManager.AssignKey<SteerLeft>(KeyboardButton::ArrowLeft, 0);
	inputManager.AssignKey<SteerRight>(KeyboardButton::ArrowRight, 0);
	inputManager.AssignKey<Fire>(KeyboardButton::SPACE, 0);
	//
	inputManager.AssignKey<SteerLeft>(KeyboardButton::A, 1);
	inputManager.AssignKey<SteerRight>(KeyboardButton::D, 1);
	inputManager.AssignKey<Fire>(KeyboardButton::S, 1);

	//Versus
	inputManager.AssignKey<BossTractorBeam>(KeyboardButton::Z, 1);
	inputManager.AssignKey<BossDive>(KeyboardButton::X, 1);
	inputManager.AssignKey<BossShoot>(KeyboardButton::C, 1);
}

void dae::Minigin::Cleanup()
{
	Locator::FreeResources();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	Mix_Quit();
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	StageManager::GetInstance().SetWindowSurface(SDL_GetWindowSurface(m_Window));

	auto& renderer{ Renderer::GetInstance() };
	auto& sceneManager{ SceneManager::GetInstance() };
	auto& input{ InputManager::GetInstance() };

	bool doContinue{ true };
	auto lastTime{ high_resolution_clock::now() };

	std::thread soundThread(&AudioService::Update, &Locator::GetAudio());

	BindCommands();
	StageManager::GetInstance().InitMenuScreen();

	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;

		input.ProcessInput();
		input.ControllerAnalogs();
		input.InputHandler();
		doContinue = input.KeyboardInput();
		sceneManager.Update();
		renderer.Render();
		EngineTime::GetInstance().SetDeltaTime(deltaTime);
		StageManager::GetInstance().Update();
	}

	soundThread.detach();

	Cleanup();
}