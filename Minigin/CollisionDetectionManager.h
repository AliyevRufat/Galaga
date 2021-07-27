#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "ComponentIncludes.h"

class TransformComponent;

class CollisionDetectionManager final : public dae::Singleton<CollisionDetectionManager>
{
public:
	void Update();
	void AddCollisionGameObject(const std::shared_ptr<GameObject>& gameObject);
	void DeleteCollisionGameObject(const std::shared_ptr<GameObject>& gameObject);
	void ClearCollisions();
private:
	friend class dae::Singleton<CollisionDetectionManager>;
	//Methods
	bool IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2);
	CollisionDetectionManager() = default;
	//Datamembers
	std::vector<TransformComponent*> m_pOtherEntityTransforms;
	TransformComponent* m_pGyaragaTransform = nullptr;
	//
	std::vector<std::shared_ptr<GameObject>> m_pOtherEntities;
	std::shared_ptr<GameObject> m_pGyaraga;
};
