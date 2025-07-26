#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"
#include "BoxCollider.h"

//honestly this is shaping more out to be a player than a camera ;-;
class Camera
{
	public:
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);
		
		//controls
		bool firstClick = false;
		bool wasMouseClicked = false;

		bool firstSpace = false;
		bool wasSpaceClicked = false;
		bool alreadyJumped = false;
		
		//Tilt
		float currentTiltSpeed = 0.0f;
		float tiltSpeed = 0.1f;
		float maxTilt = 2.5f;
		bool tiltLeft;
		bool tiltRight;

		//controls

		int width;
		int height;

		float speed = 15.0f;
		float sensitivity = 300.0f;

		float maxLeftTilt = 10.0f;
		float maxRightTilt = -10.0f;

		float TiltSpeed = 0.01f;
		
		//this is for gravity
		float Acceleration = 25.0f;
		float JumpVelocity = 15.0f;
		float Velocity = 0.0f;
		float maxVelocity = 200.0f;

		Camera(int width, int height, glm::vec3 position);
		void RigidBody(float deltaTime);

		void Matrix(Shader& shader, const char* uniform);
		void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

		void Inputs(GLFWwindow* window, float deltaTime);
		void CollisionPush(BoxCollider* collider);
};

#endif
