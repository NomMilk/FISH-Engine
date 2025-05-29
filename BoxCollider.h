#pragma once
#include <glm/glm.hpp>
#include <algorithm>

struct CollisionResult {
	bool collided;
	float pushX;
	float pushZ;
};

class BoxCollider
{
	public:
		BoxCollider(float x, float y, float Height, float Width);
		CollisionResult CheckCollision(float x, float y);

		const glm::vec4 GetBounds() const { return ColliderVertex; };

	private:
		glm::vec4 ColliderVertex;
};
