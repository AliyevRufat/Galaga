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
	bool IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2);
	CollisionDetectionManager() = default;
	//Datamembers
	std::vector<TransformComponent*> m_pOtherEntityTransforms;
	TransformComponent* m_pGyaragaTransform = nullptr;
	//
	std::pair<std::vector<std::shared_ptr<GameObject>>, std::vector<bool>> m_pOtherEntities;//bool is used to know when to delete the object
	std::shared_ptr<GameObject> m_pGyaraga;
};
