#include "BoxCollider.h"

BoxCollider::BoxCollider(float x, float y, float Height, float Width)
	:ColliderVertex(glm::vec4(x, y, Height, Width))
{}

bool BoxCollider::CheckCollision(float x, float y)
{
	return
	(
		x < ColliderVertex.x + ColliderVertex.w &&
		x > ColliderVertex.x &&
		y < ColliderVertex.y + ColliderVertex.z &&
		y > ColliderVertex.y
	);
}
