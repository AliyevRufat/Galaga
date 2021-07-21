#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:

		void Add(const std::shared_ptr<GameObject>& spObject);

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::string& GetName() const;
		void Update();
		void Render() const;

		std::shared_ptr<GameObject> GetObjectByName(const std::string& name) const;
		void AddPlayer(const std::shared_ptr<GameObject>& spPlayer);
		std::shared_ptr<GameObject> GetPlayer(int index);
		//
		void SetCurrentLevel(const std::shared_ptr<GameObject>& level);
		std::shared_ptr<GameObject> GetCurrentLevel() const;
		void AddLevel(const std::shared_ptr<GameObject>& level);
		std::shared_ptr<GameObject> GetLevel(int index) const;
		//
		float  GetSceneScale() const;
		//
		bool AreAllObjectsActive() const;
		void SetObjectsIsActive(bool isActive);
		void ClearScene();

		void DeleteMarkedObjects();
	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_SpObjects;
		std::vector<std::shared_ptr<GameObject>> m_SpPlayers;
		std::vector< std::shared_ptr<GameObject>> m_SpLevels;
		std::shared_ptr<GameObject> m_SpCurrentLevel;

		static unsigned int m_IdCounter;
		const float m_SceneScale = 2.0f;

		bool m_AreAllObjectsActive;
	};
}
