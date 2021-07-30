#include "MiniginPCH.h"
#include "BezierPathManager.h"
#include <glm\vec2.hpp>
#include "TransformComponent.h"
#include "FormationManager.h"
#include "BezierPath.h"

glm::vec2 Normalize(const glm::vec2& vector)
{
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	if (length != 0)
		return glm::vec2(vector.x / length, vector.y / length);
	else
		return vector;
}

BezierPathManager::BezierPathManager()
	:m_CurrentPath{ 0 }
	, m_CurrentWayPoint{ 0 }
	, EPSILON{ 5.0f }
{
}

glm::vec2 BezierPathManager::CalculatePath(const glm::vec2& pos)
{
	auto d = m_Paths[m_CurrentPath][m_CurrentWayPoint] - pos;

	float magnitudeSqr = sqrt(d.x * d.x + d.y * d.y);

	if (magnitudeSqr < EPSILON)
	{
		++m_CurrentWayPoint;
	}

	if (m_CurrentWayPoint < m_Paths[m_CurrentPath].size())
	{
		const float speed = 300.0f;
		glm::vec2 distance = m_Paths[m_CurrentPath][m_CurrentWayPoint] - pos;
		return pos + Normalize(distance) * EngineTime::GetInstance().GetDeltaTime() * speed;
	}
	else
	{
		return glm::vec2{ 0,0 };
	}
}

void BezierPathManager::CreatePaths(BezierPath* path)
{
	int currentPath = 0;
	m_Paths.push_back(std::vector<glm::vec2>());
	path->Sample(&m_Paths[currentPath]);
}