#pragma once
#include "SceneObject.h"
#include <vector>
#include <memory>

class BaseComponent;
class Observer;
class Subject;
class Component;
class Texture2D;

class GameObject final : public dae::SceneObject
{
public:

	template <typename T>
	T* GetComponent() const
	{
		for (auto component : m_pComponents)
		{
			if (dynamic_cast<T*>(component))
			{
				return (T*)component;
			}
		}
		return nullptr;
	}

	void Update() override;
	void Render() const override;

	void Notify(const std::string& event);

	void AddComponent(BaseComponent* pComponent);
	void AddWatcher(Observer* pObserver);

	GameObject(const std::string& name);
	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	std::vector<BaseComponent*> m_pComponents;
	std::unique_ptr<Subject> m_pActorChanged;
};
