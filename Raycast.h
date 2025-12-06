#pragma once
#include <glm/glm.hpp>
#include <algorithm>

class Raycast 
{
	public:
		Raycast(glm::vec3 _startingPos, glm::vec3 _direction) : startingPos(_startingPos), direction(_direction)
		{}
		glm::vec3 GetStartingPos() {return startingPos;}
		glm::vec3 GetDirection() {return direction;}

	private:
		glm::vec3 startingPos;
		glm::vec3 direction;
};
