#include "Camera.h"
#include "BoxCollider.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	if (currentTilt != 0.0f)
	{
		glm::mat4 baseview = glm::lookAt(Position, Position + Orientation, Up);
		glm::mat4 tilt = glm::rotate(glm::mat4(1.0f), currentTilt, glm::vec3(0.0f, 0.0f, 1.0f));
		view = tilt * baseview;
	}
	else
	{
		view = glm::lookAt(Position, Position + Orientation, Up);
	}
	// there was a problem where the aspect ratio was using int division so i updated this
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::RigidBody(float deltaTime)
{
	Position.y -= Velocity * deltaTime;
	//max velocity
	if (Velocity >= maxVelocity) return;
	Velocity += Acceleration * deltaTime;
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * forward * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position -= speed * forward * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position -= speed * side * deltaTime;
		currentTilt = -tiltAmount;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * side * deltaTime;
		currentTilt = tiltAmount;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
	{
		currentTilt = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		currentTilt = 0.0f;
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
