#pragma once
#include <glm/glm.hpp>
#include <algorithm>

struct CollisionResult {
	bool collided;
	float pushX;
	float pushY;
	float pushZ;
};

class BoxCollider
{
	public:
		BoxCollider(float x, float y, float Z, float XScale, float YScale, float ZScale);
		CollisionResult CheckCollision(float x, float y, float z);

	private:
		glm::vec3 ColliderVertex;
		glm::vec3 ColliderScale;
};
