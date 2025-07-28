#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include <stb/stb_image.h>

#include "Texture.h"
#include "BoxCollider.h"
#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "SoundManager.h"
#include "Model.h"
#include "modelLoader.h"

const unsigned int width = 1000;
const unsigned int height = 800;

Camera* globalCamera = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    
    if (globalCamera != nullptr) {
        globalCamera->width = width;
        globalCamera->height = height;
        globalCamera->updateMatrix(90.0f, 0.1f, 100.0f);
    }
}

//VAO util
//THIS ONLY WORKS FOR STATIC OBJECTS, THIS DOES NOT DELETE THE VAO WHEN IT'S NOT USED!!!
VBO VAOLinker(GLfloat* vertices, size_t vertexSize, GLuint* indices, size_t indexSize)
{
	VBO vbo(vertices, vertexSize);
	EBO ebo(indices, indexSize);
	
	// In OpenGL 2.1, we don't use VAOs, so we just return the VBO
	// Attribute setup will be done before drawing
	
	return vbo;
}

VBO VAOLinkerTexture(GLfloat* vertices, size_t vertexSize, GLuint* indices, size_t indexSize)
{
	VBO textureVBO(vertices, vertexSize);
	EBO textureEBO(indices, indexSize);
	
	// In OpenGL 2.1, we don't use VAOs, so we just return the VBO
	// Attribute setup will be done before drawing
	
	return textureVBO;
}

void DrawTriVAO(VBO& vbo, EBO& ebo, size_t indices, bool hasTexture)
{
	vbo.Bind();
	ebo.Bind();
	
	// Set up vertex attributes using shader attributes instead of fixed-function pipeline
	// This works with OpenGL 2.1 via shaders
	if (hasTexture) {
		// For textured vertices (position, color, texcoords)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	else {
		// For regular vertices (position, color)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	
	// Disable vertex arrays
	if (hasTexture) {
		glDisableVertexAttribArray(2);
	}
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
	ebo.Unbind();
	vbo.Unbind();
}

//actual Game
int main()
{
	BoxCollider GroundCollider(-25.0f, -25.0f, -5.0f, 50.0f, 50.0f, 5.0f);
	SoundManager soundPlayer("rainbowdash.mp3");
	soundPlayer.Loop();
	soundPlayer.Play();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	//vertices position 
	GLfloat textureVertices[] =
	{
		//position//				//color//
		5.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		5.0f, 2.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		6.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		6.0f, 2.0f, 0.0f,		1.0f, 1.0f, 1.0f
	};

	GLuint textureIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	GLFWwindow* window = glfwCreateWindow(width, height, "Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGL();

	glViewport(0, 0, width, height);

	Shader shaderProgram("Shaders/Default.vert", "Shaders/Default.frag");

	VBO textureVBO = VAOLinker(textureVertices, sizeof(textureVertices), textureIndices, sizeof(textureIndices));
	EBO textureEBO(textureIndices, sizeof(textureIndices));
	glm::vec4 lightColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	globalCamera = &camera;
	
	// load models from XML
	ModelLoader modelLoader;
	if (!modelLoader.loadFromXML("example.xml")) {
		std::cerr << "Failed to load models from XML file" << std::endl;
	} else {
		std::cout << "Successfully loaded " << modelLoader.getModelCount() << " models from XML" << std::endl;
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
		camera.updateMatrix(90.0f, 0.1f, 100.0f);

		camera.RigidBody(deltaTime);
		camera.CollisionPush(&GroundCollider);

		camera.Matrix(shaderProgram, "camMatrix");
		
		glm::mat4 defaultModel = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(defaultModel));
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		
		DrawTriVAO(textureVBO, textureEBO, 6, false);
		
		modelLoader.drawModels(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	globalCamera = nullptr;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
