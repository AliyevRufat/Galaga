#pragma once
#include <glm\vec2.hpp>
#include <vector>

class BezierPath;

class BezierPathManager
{
public:
	BezierPathManager(int speed);
	glm::vec2 CalculatePath(const glm::vec2& pos);
	void CreatePaths(BezierPath* path);

private:
	std::vector<std::vector<glm::vec2>> m_Paths;
	//
	int m_CurrentPath;
	int m_CurrentWayPoint;
	const float EPSILON;
	const int m_Speed;
};
