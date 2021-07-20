#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>

class BaseComponent;
class Observer;
class Subject;
class Component;
class Texture2D;

class GameObject final
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

	GameObject(const std::string& name);
	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	void Update();
	void Render() const;

	void Notify(const std::string& event);

	void AddComponent(BaseComponent* pComponent);
	void AddWatcher(Observer* pObserver);

	const std::string& GetName() const { return m_Name; };
	void SetIsActive(bool isActive) { m_IsActive = isActive; }
	bool GetIsActive() const { return m_IsActive; }

	void SetMarkForDelete(bool markFordelete) { m_MarkForDelete = markFordelete; }
	bool GetMarkForDelete() const { return m_MarkForDelete; }

private:
	std::vector<BaseComponent*> m_pComponents;
	std::unique_ptr<Subject> m_pActorChanged;

	bool m_IsActive;
	bool m_MarkForDelete;
	const std::string m_Name;
};
