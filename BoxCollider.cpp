#include "BoxCollider.h"

BoxCollider::BoxCollider(float x, float y, float Height, float Width)
	:ColliderVertex(glm::vec4(x, y, Height, Width))
{}

CollisionResult BoxCollider::CheckCollision(float x, float y)
{
    CollisionResult result;
    result.collided = (
        x < ColliderVertex.x + ColliderVertex.w &&
        x > ColliderVertex.x &&
        y < ColliderVertex.y + ColliderVertex.z &&
        y > ColliderVertex.y
        );

    if (result.collided) {
        float left = x - ColliderVertex.x;
        float right = (ColliderVertex.x + ColliderVertex.w) - x;
        float top = y - ColliderVertex.y;
        float bottom = (ColliderVertex.y + ColliderVertex.z) - y;

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
        result.pushZ = 0.0f;
    }

    return result;
}
