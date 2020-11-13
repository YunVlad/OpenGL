#include "camera.h"

camera::camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
{
    cameraPos = pos;

    worldUp = up;

    angleYaw = yaw;
    anglePitch = pitch;

    updateCamera();
}

void camera::updateCamera()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(angleYaw)) * cos(glm::radians(anglePitch));
    direction.y = sin(glm::radians(anglePitch));
    direction.z = sin(glm::radians(angleYaw)) * cos(glm::radians(anglePitch));
    cameraFront = glm::normalize(direction);
    //std::cout << angleYaw << " " << anglePitch  << std::endl;

    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

glm::mat4 camera::mat4getView()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void camera::setCameraSpeed(float time)
{
    deltaTime = time - lastTime;
    lastTime = time;
    cameraSpeed = movementSpeed * deltaTime;
}

void camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    angleYaw += xoffset;
    anglePitch += yoffset;

    if (anglePitch > 89.0f)
        anglePitch = 89.0f;
    if (anglePitch < -89.0f)
        anglePitch = -89.0f;

    updateCamera();
}

void camera::ProcessKeyboardMovement(movementType direction)
{
    if (direction == FORWARD)
        cameraPos += cameraFront * cameraSpeed;
    if (direction == BACKWARD)
        cameraPos -= cameraFront * cameraSpeed;
    if (direction == LEFT)
        cameraPos -= cameraRight * cameraSpeed;
    if (direction == RIGHT)
        cameraPos += cameraRight * cameraSpeed;
}
