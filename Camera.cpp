#include "Camera.h"
#include "BoxCollider.h"
#include <iostream>
#include <cmath>

// chat gipity helped me with interpolation

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	
	view = glm::lookAt(Position, Position + Orientation, Up);
	
	if (currentTilt != 0.0f)
	{
		glm::mat4 tilt = glm::rotate(glm::mat4(1.0f), currentTilt, glm::vec3(0.0f, 0.0f, 1.0f));
		view = tilt * view;
	}
	
	// there was a problem where the aspect ratio was using int division so i updated this
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::RigidBody(float deltaTime)
{
	Position.y -= Velocity * deltaTime;
	//max velocity
	if (Velocity < maxVelocity) {
		Velocity += Acceleration * deltaTime;
	}
	
	// apply horizontal movement velocity
	Position += moveVelocity * deltaTime;
	
	// apply friction to slow down when not actively moving
	float frictionFactor = 1.0f - std::min(moveFriction * deltaTime, 1.0f);
	moveVelocity.x *= frictionFactor;
	moveVelocity.z *= frictionFactor;
	
	// clamp very small velocities to zero to prevent sliding
	if (glm::length(glm::vec2(moveVelocity.x, moveVelocity.z)) < 0.01f) {
		moveVelocity.x = 0.0f;
		moveVelocity.z = 0.0f;
	}
	
	// for smoother interpolation between two values
	if (fabs(currentTilt - targetTilt) > 0.0001f) {
		float factor = std::min(1.0f, tiltSpeed * deltaTime);
		currentTilt = lerp(currentTilt, targetTilt, factor);
	}else if (currentTilt != targetTilt) {
		currentTilt = targetTilt;
	}
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, float deltaTime)
{
	//beware shit code incoming
	//but i'm too dumb to make it good

	//Camera Movement
	if (firstClick)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		
		//this is to prevent it from going all the way back over if you look up
		//but like I don't have the issue when I commnted it out ???
		//i'll just keep it since they kept it in the yt tutorial
		if (!(glm::angle(newOrientation, Up)) <= glm::radians(5.0f) or glm::angle(newOrientation, -Up) <= glm::radians(5.0f))
		{
			Orientation = newOrientation;
		}
		
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}

	glm::vec3 forward = glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
	glm::vec3 side = glm::normalize(glm::cross(forward, Up));
	
	glm::vec3 moveDir = glm::vec3(0.0f);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveDir += forward;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveDir -= forward;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveDir -= side;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveDir += side;
	}
	
	// normalize the movement direction if moving diagonally
	if (glm::length(moveDir) > 0.0f)
	{
		moveDir = glm::normalize(moveDir);
		moveVelocity += moveDir * moveAcceleration * deltaTime;
		float horizontalSpeed = glm::length(glm::vec2(moveVelocity.x, moveVelocity.z));
		if (horizontalSpeed > maxMoveSpeed)
		{
			moveVelocity.x = (moveVelocity.x / horizontalSpeed) * maxMoveSpeed;
			moveVelocity.z = (moveVelocity.z / horizontalSpeed) * maxMoveSpeed;
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		targetTilt = -tiltAmount;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		targetTilt = tiltAmount;
	}
	else
	{
		targetTilt = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (alreadyJumped) return;
		if (!firstSpace) firstSpace = true;
		if (wasSpaceClicked) return;
		Velocity = -JumpVelocity;
		wasSpaceClicked = true;
		alreadyJumped = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		wasSpaceClicked = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!firstClick) firstClick = true;
		if (wasMouseClicked) return;
		wasMouseClicked = true;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		wasMouseClicked = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		firstClick = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Camera::CollisionPush(BoxCollider* collider)
{
	CollisionResult result = collider->CheckCollision(Position.x, Position.z, Position.y);
	
	if (result.collided)
	{
		Position += glm::vec3(result.pushX, result.pushY, result.pushZ);

		//if touched ground
		if (result.pushZ >= 0)
		{
			Velocity = 1.0f;
			alreadyJumped = false;
		}
	}
}
