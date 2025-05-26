#pragma once
#include <glm/glm.hpp>

class BoxCollider
{
	public:
		BoxCollider(float x, float y, float Height, float Width);
		bool CheckCollision(float x, float y);

	private:
		glm::vec4 ColliderVertex;
};
