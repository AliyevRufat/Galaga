#pragma once
#include "BaseComponent.h"
#include <glm\vec2.hpp>

class GameObject;

class TractorBeamComponent final : public BaseComponent
{
public:

	TractorBeamComponent();
	void Update() override;
	void CreateTractorBeam();
	bool GetIsFinished() const;
	void Clean();
	//
	void SpawnAFighter(const glm::vec2& playerPos);
	bool GetIsPlayerCaught() const;
private:
	void UpdateTractorBeam();
	void UpdateCaughtPlayer();
	//
	std::shared_ptr<GameObject> m_pTractorBeam;
	std::shared_ptr<GameObject> m_pCaughtPlayer;

	bool m_IsFinished;
	bool m_IsCollisionAdded;
	bool m_IsPlayerCaught;
};
