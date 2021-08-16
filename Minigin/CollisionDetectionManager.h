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
	void DeleteCollidedObjects();
	void DeleteSpecificObject(const std::shared_ptr<GameObject>& object);
	void ClearCollisions();
private:
	friend class dae::Singleton<CollisionDetectionManager>;
	//Methods
	void AddExplosionEffect(int enemyIndex) const;
	void IncreasePlayerScore(const std::shared_ptr<GameObject>& bullet, const std::shared_ptr<GameObject>& gameObject);
	bool IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2);
	//
	CollisionDetectionManager() = default;
	//Datamembers
	std::vector<TransformComponent*> m_pOtherEntityTransforms;
	TransformComponent* m_pGyaragaTransform = nullptr;
	TransformComponent* m_pGyaragaTransform2 = nullptr;
	//
	std::vector<std::pair<std::shared_ptr<GameObject>, bool>> m_pOtherEntities;
	std::shared_ptr<GameObject> m_pGyaraga;
	std::shared_ptr<GameObject> m_pGyaraga2;
};
