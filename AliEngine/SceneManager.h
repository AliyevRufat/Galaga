#pragma once
#include "Singleton.h"
#include <string>
#include <memory>
#include <vector>
#include <glm\vec2.hpp>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void ClearScene(const std::shared_ptr<dae::Scene>& spScene);

		void Update();
		void Render();

		std::shared_ptr<Scene> GetSceneByName(const std::string& n) const;
		std::shared_ptr<Scene> GetCurrentScene();
		void SetCurrentScene(const std::shared_ptr<Scene>& spScene);
		//
		void SetScreenDimensions(glm::vec2 screenDim) { m_Width = screenDim.x; m_Height = screenDim.y; }
		glm::vec2 GetScreenDimensions() const { return glm::vec2{ m_Width,m_Height }; }
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_SpScenes;
		std::shared_ptr<Scene> m_SpCurrentScene;
		//
		float m_Width = 0;
		float m_Height = 0;
	};
}
