#include <iostream>
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
	GLfloat vertices[] =
	{
		-2.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.1f, 0.5f, 0.7f,
		2.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.1f, 0.5f, 0.7f,
		2.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 0.1f, 0.5f, 0.7f,
		-2.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.1f, 0.5f, 0.2f,
		2.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.1f, 0.5f, 0.2f,
		2.0, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.1f, 0.5f, 0.2f,
	};

	GLuint indices[] =
	{
		0, 3, 5,
		3, 2, 4,
		5, 4, 1
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

	Shader shaderProgram("default.vert", "default.frag");

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

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		camera.Matrix(shaderProgram, "camMatrix");

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}