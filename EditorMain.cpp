#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <cstring>
#include <float.h>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>

#include <stb/stb_image.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"

#include "Texture.h"
#include "BoxCollider.h"
#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "EditorCamera.h"
#include "SoundManager.h"
#include "Model.h"
#include "modelLoader.h"

const unsigned int width = 1000;
const unsigned int height = 800;

EditorCamera* globalCamera = nullptr;

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
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(width, height, "Editor", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGL();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();

	glViewport(0, 0, width, height);

	Shader shaderProgram("Shaders/Default.vert", "Shaders/Default.frag");

	glm::vec4 lightColor = glm::vec4(0.95f, 0.95f, 0.9f, 1.0f);
 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	EditorCamera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	globalCamera = &camera;
	
	ModelLoader modelLoader;
	std::string currentXML = "prefabRooms/example.xml";
	if (!modelLoader.loadFromXML(currentXML)) {
		std::cerr << "Failed to load models from XML file" << std::endl;
	} else {
		std::cout << "Successfully loaded " << modelLoader.getModelCount() << " models from XML" << std::endl;
	}

	bool showOpenScene = false;
	std::vector<std::string> xmlFiles;
	std::string path = std::filesystem::current_path().string() + "/prefabRooms";
	int selectedModelIndex = -1;

	//find all prefab rooms
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".xml")
		{
			std::string file = "prefabRooms/" + entry.path().filename().string();
			xmlFiles.push_back(file);
		}
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

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open Scene")) {
					showOpenScene = true;
				}
				if (ImGui::MenuItem("Save Scene")) {
					modelLoader.saveToXML(currentXML);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::SetNextWindowSize(ImVec2(1200, 800), ImGuiCond_Once);
		ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
		ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		ImGui::PopStyleVar();
		
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImVec2 viewportPos = ImGui::GetWindowPos();
		ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
		bool viewportHovered = ImGui::IsWindowHovered();
		ImGuiIO& io = ImGui::GetIO();
		bool isDragging = io.MouseDown[0] && ImGui::IsAnyItemActive();
		
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		
		int viewportX = (int)(viewportPos.x + contentMin.x);
		int viewportY = fbHeight - (int)(viewportPos.y + contentMin.y + viewportSize.y);
		int viewportW = (int)viewportSize.x;
		int viewportH = (int)viewportSize.y;
		
		if (viewportW < 1) viewportW = 1;
		if (viewportH < 1) viewportH = 1;

		ImGui::End();
		ImGui::Begin("Hierarchy");
		if (ImGui::Button("Add Model")) {
			modelLoader.addModel("Model");
			selectedModelIndex = modelLoader.getModelCount() - 1;
		}
		for (int i = 0; i < modelLoader.getModelCount(); i++) {
			const ModelData* modelData = modelLoader.getModelData(i);
			if (modelData != nullptr) {
				std::string displayName;
				if (modelData->name.empty()) {
					displayName = modelData->path;
				} else {
					displayName = modelData->name;
				}
				bool isSelected = (selectedModelIndex == i);
				if (ImGui::Selectable(displayName.c_str(), isSelected)) {
					selectedModelIndex = i;
				}
			}
		}
		ImGui::End();
		ImGui::Begin("Properties");
		if (selectedModelIndex >= 0) {
			ModelData* modelData = modelLoader.getModelData(selectedModelIndex);
			if (modelData != nullptr) {
				char nameBuffer[256];
				for (int i = 0; i < 256; i++) {
					nameBuffer[i] = 0;
				}
				for (int i = 0; i < modelData->name.length() && i < 255; i++) {
					nameBuffer[i] = modelData->name[i];
				}
				if (ImGui::InputText("Name", nameBuffer, 256)) {
					modelData->name = nameBuffer;
				}
				
				char pathBuffer[512];
				for (int i = 0; i < 512; i++) {
					pathBuffer[i] = 0;
				}
				for (int i = 0; i < modelData->path.length() && i < 511; i++) {
					pathBuffer[i] = modelData->path[i];
				}
				if (ImGui::InputText("Path", pathBuffer, 512)) {
					modelData->path = pathBuffer;
					modelLoader.loadModelForIndex(selectedModelIndex);
				}
				
				float posX = modelData->position.x;
				float posY = modelData->position.y;
				float posZ = modelData->position.z;
				float posArray[3] = {posX, posY, posZ};
				if (ImGui::InputFloat3("Position", posArray)) {
					modelData->position.x = posArray[0];
					modelData->position.y = posArray[1];
					modelData->position.z = posArray[2];
				}
				
				float rotX = modelData->rotation.x;
				float rotY = modelData->rotation.y;
				float rotZ = modelData->rotation.z;
				float rotArray[3] = {rotX, rotY, rotZ};
				if (ImGui::InputFloat3("Rotation", rotArray)) {
					modelData->rotation.x = rotArray[0];
					modelData->rotation.y = rotArray[1];
					modelData->rotation.z = rotArray[2];
				}
				
				float scaleX = modelData->scale.x;
				float scaleY = modelData->scale.y;
				float scaleZ = modelData->scale.z;
				float scaleArray[3] = {scaleX, scaleY, scaleZ};
				if (ImGui::InputFloat3("Scale", scaleArray)) {
					modelData->scale.x = scaleArray[0];
					modelData->scale.y = scaleArray[1];
					modelData->scale.z = scaleArray[2];
				}
			}
		}
		ImGui::End();

		if (showOpenScene) {
			ImGui::Begin("Open Scene", &showOpenScene);
			for (size_t i = 0; i < xmlFiles.size(); i++) {
				if (ImGui::Selectable(xmlFiles[i].c_str())) {
					currentXML = xmlFiles[i];
					modelLoader.loadFromXML(currentXML);
					selectedModelIndex = -1;
					showOpenScene = false;
				}
			}
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		if (viewportW > 0 && viewportH > 0) {
			GLint oldViewport[4];
			GLboolean oldScissorEnabled;
			GLint oldScissorBox[4];
			glGetIntegerv(GL_VIEWPORT, oldViewport);
			glGetBooleanv(GL_SCISSOR_TEST, &oldScissorEnabled);
			if (oldScissorEnabled) {
				glGetIntegerv(GL_SCISSOR_BOX, oldScissorBox);
			}
			
			glViewport(viewportX, viewportY, viewportW, viewportH);
			glEnable(GL_SCISSOR_TEST);
			glScissor(viewportX, viewportY, viewportW, viewportH);
			
			camera.width = viewportW;
			camera.height = viewportH;
			
			bool mouseOverOtherUI = io.WantCaptureMouse && !viewportHovered;
			
			if (!mouseOverOtherUI && !isDragging) {
				camera.Inputs(window, deltaTime);
			}
			camera.updateMatrix(90.0f, 0.1f, 100.0f);

			glEnable(GL_DEPTH_TEST);
			glClearColor(0.549f, 0.671f, 0.631f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaderProgram.Activate();
			camera.Matrix(shaderProgram, "camMatrix");
			
			glm::mat4 defaultModel = glm::mat4(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(defaultModel));

			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(defaultModel)));
			glUniformMatrix3fv(glGetUniformLocation(shaderProgram.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos[0]"), 0.0f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos[1]"), moving_Test, 0.2f, 0.2f);
			
			modelLoader.drawModels(shaderProgram);
			glUseProgram(0);
			
			if (!oldScissorEnabled) {
				glDisable(GL_SCISSOR_TEST);
			} else {
				glScissor(oldScissorBox[0], oldScissorBox[1], oldScissorBox[2], oldScissorBox[3]);
			}
			glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	globalCamera = nullptr;
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
