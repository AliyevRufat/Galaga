#include "MiniginPCH.h"
#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(const unsigned int& score)
	: m_Score{ score }
{
}

void ScoreComponent::SetScore(const unsigned int& score)
{
	m_Score = score;
	m_pGameObject->Notify(Event::ScoreGained);
}

void ScoreComponent::IncreaseScore(const int score)
{
	m_Score += score;
	m_pGameObject->Notify(Event(score)); //TODO : incomplete, event doesn't give the int of score anymore, find another way
}

const unsigned int& ScoreComponent::GetScore() const
{
	return m_Score;
}