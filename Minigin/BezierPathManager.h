#pragma once
#include <glm\vec2.hpp>
#include <vector>

class BezierPath;

class BezierPathManager final
{
public:
	BezierPathManager(int speed);
	glm::vec2 CalculatePath(const glm::vec2& pos);
	void CreatePaths(BezierPath* path);

private:
	std::vector<std::vector<glm::vec2>> m_Paths;
	//
	int m_CurrentPath;
	size_t m_CurrentDest;
	const float m_Offset;
	const int m_Speed;
};
