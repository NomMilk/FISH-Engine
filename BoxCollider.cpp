#include "BoxCollider.h"

BoxCollider::BoxCollider(float x, float y, float z, float Height, float Width, float Depth)
	:ColliderVertex(glm::vec3(x, y, z)),
     ColliderScale(glm::vec3(Height, Width, Depth))
{}

CollisionResult BoxCollider::CheckCollision(float x, float y, float z)
{
    CollisionResult result;
    result.collided = (
        x < ColliderVertex.x + ColliderScale.x &&
        x > ColliderVertex.x &&
        y < ColliderVertex.y + ColliderScale.y &&
        y > ColliderVertex.y &&
        z < ColliderVertex.z + ColliderScale.z &&
        z > ColliderVertex.z
        );

    if (result.collided) {
        float left = x - ColliderVertex.x;
        float right = (ColliderVertex.x + ColliderScale.x) - x;
        float top = y - ColliderVertex.y;
        float bottom = (ColliderVertex.y + ColliderScale.y) - y;
        float front = z - ColliderVertex.z;
        float back = (ColliderVertex.z + ColliderScale.z) - z;;

        if (std::min(left, right) < std::min(top, bottom)) {
            result.pushX = (left < right) ? -left : right;
            result.pushZ = 0.0f;
        }
        else {
            result.pushX = 0.0f;
            result.pushZ = (top < bottom) ? -top : bottom;
        }
    }
    else {
        result.pushX = 0.0f;
        result.pushY = 0.0f;
        result.pushZ = 0.0f;
    }

    return result;
}
