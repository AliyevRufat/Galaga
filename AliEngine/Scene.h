#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:

		enum class GameMode
		{
			None,
			SinglePlayer,
			Coop,
			Versus
		};
		enum class Level
		{
			FirstLevel = 0,
			SecondLevel = 1,
			ThirdLevel = 2,
			Finished = 3
		};

		void Add(const std::shared_ptr<SceneObject>& spObject);

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::string& GetName() const;
		void Update();
		void Render() const;

		std::shared_ptr<SceneObject> GetObjectByName(const std::string& name) const;
		void AddPlayer(const std::shared_ptr<GameObject>& spPlayer);
		std::shared_ptr<GameObject> GetPlayer(int index);
		//
		void SetCurrentLevel(const std::shared_ptr<GameObject>& level);
		std::shared_ptr<GameObject> GetCurrentLevel() const;
		void AddLevel(const std::shared_ptr<GameObject>& level);
		std::shared_ptr<GameObject> GetLevel(int index) const;
		//
		float  GetSceneScale() const;
		GameMode GetCurrentGameMode() const { return m_CurrentGameMode; };
		void SetCurrentGameMode(GameMode gameMode) { m_CurrentGameMode = gameMode; };
		//
		Level GetGameLevel() const { return m_CurrentLevel; }
		void SetGameLevel(Level level) { m_CurrentLevel = level; }
		//
		bool AreAllObjectsActive() const;
		void SetObjectsIsActive(bool isActive);
		void ClearScene();

		void DeleteMarkedObjects();
	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<SceneObject>> m_SpObjects;
		std::vector<std::shared_ptr<GameObject>> m_SpPlayers;
		std::vector< std::shared_ptr<GameObject>> m_SpLevels;
		std::shared_ptr<GameObject> m_SpCurrentLevel;

		static unsigned int m_IdCounter;
		const float m_SceneScale = 2.0f;
		GameMode m_CurrentGameMode;
		Level m_CurrentLevel;

		bool m_AreAllObjectsActive;
	};
}
