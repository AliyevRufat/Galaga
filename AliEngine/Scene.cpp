#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include <string>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_SpLevels{}
	, m_SpObjects{}
	, m_SpPlayers{}
	, m_AreAllObjectsActive{ true }
{
}

void Scene::Add(const std::shared_ptr<GameObject>& spObject)
{
	m_SpObjects.push_back(spObject);
}

void Scene::Update()
{
	for (size_t i = 0; i < m_SpObjects.size(); i++)
	{
		m_SpObjects[i]->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_SpObjects)
	{
		object->Render();
	}
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

std::shared_ptr<GameObject> Scene::GetObjectByName(const std::string& name) const
{
	for (const auto& object : m_SpObjects)
	{
		if (object->GetName().compare(name) == 0)
		{
			return object;
		}
	}
	return nullptr;
}

void Scene::AddPlayer(const std::shared_ptr<GameObject>& spPlayer)
{
	m_SpPlayers.push_back(spPlayer);
}

std::shared_ptr<GameObject> Scene::GetPlayer(int index)
{
	if (m_SpPlayers.size() <= (unsigned)index)
	{
		return nullptr;
	}

	return m_SpPlayers[index];
}

float dae::Scene::GetSceneScale() const
{
	return m_SceneScale;
}

void dae::Scene::DeleteMarkedObjects()
{
	for (size_t i = 0; i < m_SpObjects.size(); i++)
	{
		if (m_SpObjects[i]->GetMarkForDelete())
		{
			m_SpObjects.erase(std::remove(m_SpObjects.begin(), m_SpObjects.end(), *(m_SpObjects.begin() + i)));
			--i;
		}
	}
}

bool dae::Scene::AreAllObjectsActive() const
{
	return m_AreAllObjectsActive;
}

void dae::Scene::ClearScene()
{
	m_SpObjects.clear();
	m_SpPlayers.clear();
	m_SpLevels.clear();
}