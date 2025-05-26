#include "BoxCollider.h";

BoxCollider::BoxCollider(float LT, float LB, float RT, float RB)
	:ColliderVertex(glm::vec4(LT, LB, RT, RB))
{}

bool BoxCollider::CheckCollision(BoxCollider _collider)
{
	
}

bool BoxCollider::CheckCollision(float LT, float LB, float RT, float RB)
{

}
