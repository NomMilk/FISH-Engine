#pragma once
#include <glm/glm.hpp>
#include <algorithm>

struct CollisionResult {
	bool collided;
	float pushX;
	float pushY;
	float pushZ;

	CollisionResult():collided(false), pushX(0.0f), pushY(0.0f), pushZ(0.0f)
	{}

};

class BoxCollider
{
	public:
		BoxCollider(float x, float y, float z, float Height, float Width, float Depth);
		CollisionResult CheckCollision(float x, float y, float z);

	private:
		glm::vec3 ColliderVertex;
		glm::vec3 ColliderScale;
};
