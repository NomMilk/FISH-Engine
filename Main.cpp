#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>

#include <stb/stb_image.h>

#include "Texture.h"
#include "Raycast.h"
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

std::vector<BoxCollider*> pushbackColliders;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    
    if (globalCamera != nullptr) {
        globalCamera->width = width;
        globalCamera->height = height;
        globalCamera->updateMatrix(90.0f, 0.1f, 100.0f);
    }
}

//actual Game
int main()
{
	BoxCollider GroundCollider(-25.0f, -25.0f, -5.0f, 50.0f, 50.0f, 5.0f);
	Raycast ray(glm::vec3(0, 0, 60), glm::vec3(0.1, 0.1, -1));

	pushbackColliders.push_back(&GroundCollider);

	SoundManager soundPlayer("rainbowdash.mp3");
	soundPlayer.Loop();
	soundPlayer.Play();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

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

	glm::vec4 lightColor = glm::vec4(0.6f, 0.6f, 0.95f, 1.0f);
 
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
	float print_time = 0;
	auto lastTick = std::chrono::system_clock::now();
	float deltaTime = 0;
	float fps = 0;

	float moving_Test = 0;

	std::cout << "\033[2J\033[1;1H"; // Clear screen & move cursor to top-left

	while (!glfwWindowShouldClose(window))
	{
		GroundCollider.CheckCollision(ray);
		moving_Test += deltaTime / 2;

		auto now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - lastTick;
		deltaTime = elapsed.count();
		lastTick = now;
		print_time += deltaTime;
		
		//----FPS--//
		fps = 1.0f / deltaTime;

		if (print_time >= 1.0f) {
			std::cout << "\033[2J\033[1;1H"; // Clear screen & move cursor to top-left
			std::cout << "FPS: " << fps << '\n';
			print_time = 0.0f;
		}

		//----FPS--//

		glClearColor(0.549f, 0.671f, 0.631f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		
		//player Init
		camera.Inputs(window, deltaTime);
		camera.updateMatrix(90.0f, 0.1f, 100.0f);

		camera.RigidBody(deltaTime);
		for (BoxCollider* currentCollider : pushbackColliders)
		{
			camera.CollisionPush(currentCollider);
		}

		camera.Matrix(shaderProgram, "camMatrix");
		
		glm::mat4 defaultModel = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(defaultModel));

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(defaultModel)));
		glUniformMatrix3fv(glGetUniformLocation(shaderProgram.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos[0]"), 0.0f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos[1]"), moving_Test, 0.2f, 0.2f);
		
		modelLoader.drawModels(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	globalCamera = nullptr;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
