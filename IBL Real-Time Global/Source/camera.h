#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

enum movementType
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class camera
{

public:
	camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch);

	void updateCamera();
	glm::mat4 mat4getView();

	void setCameraSpeed(float time);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessKeyboardMovement(movementType direction);
	glm::vec3 getCameraPos();
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;

	float angleYaw;
	float anglePitch;

	float cameraSpeed;
	float movementSpeed = 3.0f;
	float mouseSensitivity = 0.1f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	float lastX = 960;
	float lastY = 540;
	bool firstMouse = true;


};

#endif //CAMERA_H
