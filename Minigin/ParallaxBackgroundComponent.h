#pragma once
#include "ComponentIncludes.h"
#include "Texture2DComponent.h"

class ParallaxBackgroundComponent final : public BaseComponent
{
public:
	ParallaxBackgroundComponent(const std::string& pngPath, int speed);
	//
	void Init();
	void Update();
private:
	std::shared_ptr<GameObject> m_Background1;
	std::shared_ptr<GameObject> m_Background2;
	//
	std::string m_PathFile;
	//
	int m_Speed;
};
