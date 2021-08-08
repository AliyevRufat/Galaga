#pragma once
#include "BaseComponent.h"
#include <glm\vec2.hpp>

class GameObject;

class TractorBeamComponent final : public BaseComponent
{
public:
	TractorBeamComponent();
	//
	void Update() override;
	void CreateTractorBeam();
	void SpawnAFighter(const glm::vec2& playerPos);

	bool GetIsFinished() const;
	bool GetIsPlayerCaught() const;

	void Clean();
private:
	void UpdateTractorBeam();
	void UpdateCaughtPlayer();
	//
	std::shared_ptr<GameObject> m_pTractorBeam;
	std::shared_ptr<GameObject> m_pCaughtPlayer;
	//
	bool m_IsFinished;
	bool m_IsCollisionAdded;
	bool m_IsPlayerCaught;
};
