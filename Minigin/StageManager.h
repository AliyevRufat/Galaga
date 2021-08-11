#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "ComponentIncludes.h"
#include <vector>

class StageManager final : public dae::Singleton<StageManager>
{
public:

	enum class Stage
	{
		One,
		Two,
		Three
	};

	void Update();
	//
	void InitStageOne();
	void InitStageTwo();
	void InitStageThree();
	//
	void InitStartScreen();
	void InitWinScreen();
	void InitGameOverScreen();

private:
	friend class dae::Singleton<StageManager>;
	StageManager() = default;
	//
	Stage m_Stage;
};
