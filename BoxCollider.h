#pragma once
#include <glm/glm.hpp>

class BoxCollider
{
	public:
		BoxCollider(float LT, float LB, float RT, float RB);

		bool CheckCollision(BoxCollider _Collider);
		bool CheckCollision(float LT, float LB, float RT, float RB);

	private:
		glm::vec4 ColliderVertex;
};
