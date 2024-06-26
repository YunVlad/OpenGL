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
    //std::cout << cameraPos[0] << " " << cameraPos[1] << " " << cameraPos[2] << std::endl;
    /*std::cout << (cameraPos + cameraFront)[0] << " " << (cameraPos + cameraFront)[1] << " " << (cameraPos + cameraFront)[2] << std::endl;
    std::cout << cameraUp[0] << " " << cameraUp[1] << " " << cameraUp[2] << std::endl;
    std::cout << std::endl << std::endl;*/
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

glm::vec3 camera::getCameraPos()
{
    return cameraPos;
}

void camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    ProcessMouseMovement(xoffset, yoffset);
}
