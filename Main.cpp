#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//vertices position 
	GLfloat otherVertices[] =
	{
	   -0.5f, 0.0f,  0.5f,  0.83f, 0.70f, 0.00f,
	   -0.5f, 0.0f, -0.5f,  0.83f, 0.70f, 0.00f,
		0.5f, 0.0f, -0.5f,  0.83f, 0.70f, 0.00f,
		0.5f, 0.0f,  0.5f,  0.83f, 0.70f, 0.00f,
		0.0f, 1.0f,  0.0f,  0.83f, 0.70f, 0.44f
	};

	GLuint otherIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	GLfloat vertices[] =
	{
	   -2.5f, 0.0f,  0.5f,  0.83f, 0.70f, 0.44f,
	   -2.5f, 0.0f, -0.5f,  0.83f, 0.70f, 0.44f,
	   -1.5f, 0.0f, -0.5f,  0.83f, 0.70f, 0.44f,
	   -1.5f, 0.0f,  0.5f,  0.83f, 0.70f, 0.44f,
	   -2.0f, 1.0f,  0.0f,  0.00f, 0.90f, 0.00f
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};


	GLFWwindow* window = glfwCreateWindow(width, height, "Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	gladLoadGL();

	glViewport(0, 0, width, height);

	

	Shader shaderProgram("Default.vert", "Default.frag");

	VAO VAO2;
	VAO2.Bind();
	
	VBO VBO2(otherVertices, sizeof(otherVertices));
	EBO EBO2(otherIndices, sizeof(otherIndices));

	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();


	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	
	auto lastTick = std::chrono::system_clock::now();
	float deltaTime = 0;

	while (!glfwWindowShouldClose(window))
	{
		auto now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - lastTick;
		deltaTime = elapsed.count();
		lastTick = now;

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		
		camera.Inputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		camera.Matrix(shaderProgram, "camMatrix");

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		VAO2.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}