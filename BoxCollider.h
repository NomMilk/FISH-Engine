#pragma once

class BoxCollider
{
	public:
		void SetEdge(float LT, float LB, float RT, float RB);
		void CheckCollision();

	private:
		float LeftTopEdge;
		float LeftBottomEdge;
		float RightTopEdge;
		float RightBottomEdge;
};
