#include "MiniginPCH.h"
#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(const unsigned int& score)
	: m_Score{ score }
{
}

void ScoreComponent::SetScore(const unsigned int& score)
{
	m_Score = score;
}

void ScoreComponent::IncreaseScore(const int score)
{
	m_Score += score;
	m_pGameObject->Notify("ScoreGained");
}

const unsigned int& ScoreComponent::GetScore() const
{
	return m_Score;
}