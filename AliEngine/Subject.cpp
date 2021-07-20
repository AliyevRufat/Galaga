#include "pch.h"
#include "Subject.h"
#include <algorithm>
#include "FPSTextComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"

Subject::Subject()
{}

Subject::~Subject()
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		delete m_pObservers[i];
	}
}

void Subject::AddObserver(Observer* observer)
{
	m_pObservers.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		if (m_pObservers[i] == observer)
		{
			delete m_pObservers[i];
			m_pObservers[i] = nullptr;
		}
	}
}

void Subject::Notify(const GameObject* actor, const std::string& event)
{
	std::for_each(m_pObservers.begin(), m_pObservers.end(), [&](auto observer) {observer->OnNotify(actor, event); });
}