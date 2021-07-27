#pragma once
#include <glm\vec2.hpp>
#include <vector>

struct BezierCurve
{
	glm::vec2 p0;
	glm::vec2 p1;
	glm::vec2 p2;
	glm::vec2 p3;

	glm::vec2 CalcCurvePoint(float t)
	{
		float tt = t * t;
		float ttt = tt * t;
		float u = 1.0f - t;
		float uu = u * u;
		float uuu = uu * u;

		glm::vec2 point = (uuu * p0) + (3 * uu * t * p1) + (3 * u * tt * p2) + (ttt * p3);
		point.x = round(point.x);
		point.y = round(point.y);
		return point;
	}
};

class BezierPath
{
public:
	BezierPath() {};
	~BezierPath() {};

	void AddCurve(BezierCurve curve, int samples)
	{
		m_BezierCurves.push_back(curve);
		m_Samples.push_back(samples);
	}

	void Sample(std::vector<glm::vec2>* sampledPath)
	{
		for (size_t i = 0; i < m_BezierCurves.size(); i++)
		{
			for (float t = 0; t <= 1.0f; t += (1.0f / m_Samples[i]))
			{
				sampledPath->push_back(m_BezierCurves[i].CalcCurvePoint(t));
			}
		}
	}

private:

	std::vector<BezierCurve> m_BezierCurves;
	std::vector<int> m_Samples;
};
