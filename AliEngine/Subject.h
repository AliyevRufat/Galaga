#pragma once
#include "GameObject.h"
#include "Observer.h"

class Subject
{
	friend class GameObject;
public:
	Subject();
	~Subject();
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

protected:
	void Notify(const GameObject* actor, Event event);

private:
	std::vector<Observer*> m_pObservers;
};
