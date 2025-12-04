#include "EditorCamera.h"
#include "BoxCollider.h"
#include <iostream>
#include <cmath>

// chat gipity helped me with interpolation

EditorCamera::EditorCamera(int width, int height, glm::vec3 position)
	:width(width),
	 height(height),
	 Position(position)
{}

void EditorCamera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	
	view = glm::lookAt(Position, Position + Orientation, Up);
	
	// there was a problem where the aspect ratio was using int division so i updated this
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void EditorCamera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void EditorCamera::Inputs(GLFWwindow* window, float deltaTime)
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
		
		if (Orientation.y > 0.98f && rotX < 0.0f) rotX = 0.0f;
		if (Orientation.y < -0.98f && rotX > 0.0f) rotX = 0.0f;
		Orientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}

	glm::vec3 forward = glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
	glm::vec3 side = glm::normalize(glm::cross(forward, Up));
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += forward;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position -= forward;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position -= side;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += side;
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
