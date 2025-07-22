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

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width/height), nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::RigidBody(float deltaTime)
{
	Velocity += Acceleration * deltaTime;
	Position.y -= Velocity * deltaTime;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, float deltaTime)
{
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
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * side * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		Position -= speed * Up * deltaTime;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!firstClick) firstClick = true;
		if (wasMouseClicked) return;
		//this code is for detecting first click
		//might be better to use the "Keycallback thingy"
		//but like i don't want to read the docs so we have this other solution
		//where we just deletcts if it releases then set it back for the OnClick event
		std::cout << "Clicked!\n";
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
	std::cout << result.collided << ',' << result.pushX << result.pushZ << '\n';
	std::cout << Position.x << ',' << Position.z << '\n';
	
	if (result.collided)
	{
		Position += glm::vec3(result.pushX, result.pushY, result.pushZ);
	}
}
