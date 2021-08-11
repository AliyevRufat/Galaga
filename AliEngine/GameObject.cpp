#include "pch.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "EngineTime.h"
#include "Subject.h"

GameObject::GameObject(const std::string& name, GameObject* parent, const glm::vec2& dimensions)
	:m_Name{ name }
	, m_pActorChanged{ new Subject() }
	, m_IsActive{ true }
	, m_MarkForDelete{ false }
	, m_pParent{ parent }
	, m_Dimensions{ dimensions }
{
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
	}
}

void GameObject::Update()
{
	if (!m_IsActive)
	{
		return;
	}

	if (m_MarkForDelete)
	{
		return;
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Update();
	}
}

void GameObject::Render() const
{
	if (!m_IsActive)
	{
		return;
	}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Render();
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	pComponent->SetOwnerObject(this);
	m_pComponents.push_back(pComponent);
}

void GameObject::AddWatcher(Observer* pObserver)
{
	m_pActorChanged->AddObserver(pObserver);
}

void GameObject::Notify(const std::string& event)
{
	m_pActorChanged->Notify(this, event);
}

GameObject* GameObject::GetParent() const
{
	if (m_pParent)
	{
		return m_pParent;
	}
	return nullptr;
}