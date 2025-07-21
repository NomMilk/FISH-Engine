#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include "Texture.h"
#include "BoxCollider.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "SoundManager.h"
#include "Model.h"

const unsigned int width = 1000;
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

VAO VAOLinkerTexture(GLfloat* vertices, size_t vertexSize, GLuint* indices, size_t indexSize)
{
	VAO textureVAO;
	textureVAO.Bind();

	VBO textureVBO(vertices, vertexSize);
	EBO textureEBO(indices, indexSize);
	
	textureVAO.LinkAttrib(textureVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	textureVAO.LinkAttrib(textureVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	textureVAO.LinkAttrib(textureVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	textureVAO.Unbind();
	textureVBO.Unbind();
	textureEBO.Unbind();
	
	return textureVAO;
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
	BoxCollider TestCollider(-2.5f, -1.0f, 0.0f, 1.0f, 1.0f, 100.0f);
	BoxCollider GroundCollider(-2.5f, -5.0f, 3.0f, 1000.0f, 1000.0f, 1000.0f);
	SoundManager soundPlayer("rainbowdash.mp3");
	soundPlayer.Loop();
	soundPlayer.Play();

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
		6.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
		6.0f, 2.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f
	};

	GLuint textureIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat groundVertices[] =
	{
	   -20.0f, -2.0f,  20.0f,  0.702f, 0.647f, 0.333f,
	   -20.0f, -2.0f, -20.0f,  0.702f, 0.647f, 0.333f,
		20.0f, -2.0f, -20.0f,  0.702f, 0.647f, 0.333f,
		20.0f, -2.0f,  20.0f,  0.702f, 0.647f, 0.333f,
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

	//first width and height is ratio and next two is the max height and width
	glfwSetWindowSizeLimits(window, width, height, width, height);
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	Shader shaderProgram("Default.vert", "Default.frag");
	
	VAO groundVAO = VAOLinker(groundVertices, sizeof(groundVertices), groundIndices, sizeof(groundIndices));
	VAO triangleVAO = VAOLinker(vertices, sizeof(vertices), indices, sizeof(indices));
	VAO textureVAO = VAOLinkerTexture(textureVertices, sizeof(textureVertices), textureIndices, sizeof(textureIndices));

 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//create / load textures
	Texture goldFish("goldfish.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	goldFish.texUnit(shaderProgram, "tex0", 0);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	
	// 3d model loader
	Model* ourModel = nullptr;
	try {
		ourModel = new Model("models/sonic.obj");
	} catch (const std::exception& e) {
		std::cerr << "Failed to load model: " << e.what() << std::endl;
	}
	
	//delta time stuff
	auto lastTick = std::chrono::system_clock::now();
	float deltaTime = 0;

	while (!glfwWindowShouldClose(window))
	{
		auto now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - lastTick;
		deltaTime = elapsed.count();
		lastTick = now;

		glClearColor(0.549f, 0.671f, 0.631f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		
		//player Init
		camera.Inputs(window, deltaTime);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		//camera.RigidBody(deltaTime);

		camera.Matrix(shaderProgram, "camMatrix");
		
		glm::mat4 defaultModel = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(defaultModel));
		
		camera.CollisionPush(&TestCollider);

		glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), false);
		DrawTriVAO(triangleVAO, 18);
		DrawTriVAO(groundVAO, 6);
		
		goldFish.Bind();
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "useTexture"), true);
		DrawTriVAO(textureVAO, 6);
		
		if (ourModel) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			try {
				ourModel->Draw(shaderProgram);
			} catch (const std::exception& e) {
				std::cerr << "Error drawing model: " << e.what() << std::endl;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (ourModel) {
		delete ourModel;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}