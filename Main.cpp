#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

//VAO util
//THIS ONLY WORKS FOR STATIC OBJECTS, THIS DOES NOT DELETE THE VAO WHEN IT'S NOT USED!!!
VAO VAOLinker(GLfloat* vertices, size_t vertexSize, GLuint* indices, size_t indexSize)
{
	VAO vao;
	vao.Bind();

	VBO vbo(vertices, vertexSize);
	EBO ebo(indices, indexSize);
	
	//bind position
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	//bind colors
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	return vao;
}

void DrawTriVAO(VAO& drawnVAO, size_t indices)
{
	drawnVAO.Bind();
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	drawnVAO.Unbind();
}

//actual Game
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//vertices position 
	GLfloat textureVertices[] =
	{
		//position//				//color//			//texture//
		5.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
		5.0f, 2.0f, 0.0f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		6.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		6.0f, 2.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f
	};

	GLuint textureIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat groundVertices[] =
	{
	   -20.0f, -2.0f,  20.0f,  0.28f, 0.75f, 0.00f,
	   -20.0f, -2.0f, -20.0f,  0.28f, 0.75f, 0.00f,
		20.0f, -2.0f, -20.0f,  0.28f, 0.75f, 0.00f,
		20.0f, -2.0f,  20.0f,  0.28f, 0.75f, 0.00f,
	};

	GLuint groundIndices[] =
	{
		0, 2, 1, 
		0, 3, 2
	};

	GLfloat vertices[] =
	{
	   -2.5f, 0.0f,  0.5f,  0.00f, 0.00f, 1.00f,
	   -2.5f, 0.0f, -0.5f,  0.00f, 0.00f, 1.00f,
	   -1.5f, 0.0f, -0.5f,  0.00f, 0.00f, 1.00f,
	   -1.5f, 0.0f,  0.5f,  0.00f, 0.00f, 1.00f,
	   -2.0f, 1.0f,  0.0f,  0.00f, 0.00f, 1.00f
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
	
	VAO groundVAO = VAOLinker(groundVertices, sizeof(groundVertices), groundIndices, sizeof(groundIndices));
	VAO triangleVAO = VAOLinker(vertices, sizeof(vertices), indices, sizeof(indices));

	VAO textureVAO;
	textureVAO.Bind();

	VBO textureVBO(textureVertices, sizeof(textureVertices));
	EBO textureEBO(textureIndices, sizeof(textureIndices));
	
	textureVAO.LinkAttrib(textureVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	textureVAO.LinkAttrib(textureVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	textureVAO.LinkAttrib(textureVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	textureVAO.Unbind();
	textureVBO.Unbind();
	textureEBO.Unbind();

 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	
	auto lastTick = std::chrono::system_clock::now();
	float deltaTime = 0;
	
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load("goldfish.jpg", &widthImg, &heightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

	while (!glfwWindowShouldClose(window))
	{
		auto now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - lastTick;
		deltaTime = elapsed.count();
		lastTick = now;

		glClearColor(0.51f, 0.784f, 0.898f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		
		camera.Inputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		camera.Matrix(shaderProgram, "camMatrix");
		
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), false);
		DrawTriVAO(triangleVAO, 18);
		DrawTriVAO(groundVAO, 6);
		

		glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), true);
		glBindTexture(GL_TEXTURE_2D, texture);
		DrawTriVAO(textureVAO, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}