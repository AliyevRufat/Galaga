#pragma once
#include "ComponentIncludes.h"

class AnimationComponent final : public BaseComponent
{
public:

	AnimationComponent(float frameSwitchTimer, int nrOfColumns, bool isLoop);
	void Animate();
	void Render() override;
	int GetNrOfColumns() const;
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	int m_NrOfColumns;
	int m_AnimIndex;
	bool m_IsLoop;
	//
	const float m_NextFrameTimer;
	float m_NextFrameTime;
};
