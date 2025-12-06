#include "BoxCollider.h"
#include <iostream>
#include <cstdlib>

BoxCollider::BoxCollider(float x, float y, float z, float Height, float Width, float Depth)
	:ColliderVertex(glm::vec3(x, y, z)),
     ColliderScale(glm::vec3(Height, Width, Depth))
{}

CollisionResult BoxCollider::CheckCollision(Raycast ray)
{
	CollisionResult result;

	//easy guards

	if(ray.GetStartingPos().x > ColliderVertex.x + ColliderScale.x && ray.GetDirection().x >= 0)
		return result;
	if(ray.GetStartingPos().x < ColliderVertex.x && ray.GetDirection().x >= 0)
		return result;
	
	if(ray.GetStartingPos().y > ColliderVertex.y + ColliderScale.y && ray.GetDirection().y >= 0)
		return result;
	if(ray.GetStartingPos().y < ColliderVertex.y && ray.GetDirection().y <= 0)
		return result;

	if(ray.GetStartingPos().z > ColliderVertex.z + ColliderScale.z && ray.GetDirection().z >= 0)
		return result;
	if(ray.GetStartingPos().z < ColliderVertex.z && ray.GetDirection().z <= 0)
		return result;
	
	//some nightmare shi (shoul've looked up a youtube video for dis)
	if(
		!(
			(	
				((ray.GetStartingPos().y - ColliderVertex.y)/ray.GetDirection().y *
				ray.GetDirection().x) + ray.GetStartingPos().x >= ColliderVertex.x + ColliderScale.x
				||	
				((ray.GetStartingPos().y - ColliderVertex.y)/ray.GetDirection().y *
				ray.GetDirection().x) + ray.GetStartingPos().x <= ColliderVertex.x
			)
			&&
			(	
				((ray.GetStartingPos().x - ColliderVertex.x)/ray.GetDirection().x *
				ray.GetDirection().y) + ray.GetStartingPos().y >= ColliderVertex.y + ColliderScale.y
				||	
				((ray.GetStartingPos().x - ColliderVertex.x)/ray.GetDirection().x *
				ray.GetDirection().y) + ray.GetStartingPos().y <= ColliderVertex.y
			)
			&&
			(	
				((ray.GetStartingPos().x - ColliderVertex.x)/ray.GetDirection().x *
				ray.GetDirection().z) + ray.GetStartingPos().z >= ColliderVertex.z + ColliderScale.z
				||	
				((ray.GetStartingPos().x - ColliderVertex.x)/ray.GetDirection().x *
				ray.GetDirection().z) + ray.GetStartingPos().z <= ColliderVertex.z
			)
		)
	  )
		return result;
	
	result.collided = true;

	return result;
}

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
        float back = (ColliderVertex.z + ColliderScale.z) - z;

        float minX = std::min(left, right);
        float minY = std::min(top, bottom);
        float minZ = std::min(front, back);

        // Find the axis with the minimum penetration
        if (minX <= minY && minX <= minZ) {
            result.pushX = (left < right) ? -left : right;
            result.pushY = 0.0f;
            result.pushZ = 0.0f;
        }
        else if (minY <= minX && minY <= minZ) {
            result.pushX = 0.0f;
            result.pushY = 0.0f;
            result.pushZ = (top < bottom) ? -top : bottom;
        }
        else {
            result.pushX = 0.0f;
            result.pushY = (front < back) ? -front : back;
			result.pushZ = 0.0f;
        }
    }
    else {
        result.pushX = 0.0f;
        result.pushY = 0.0f;
        result.pushZ = 0.0f;
    }

    return result;
}
