// Camera.cpp
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(0.05f), mouseSensitivity(0.1f), zoom(45.0f), firstMouse(true), lastX(800.0f / 2.0), lastY(600.0 / 2.0)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

void Camera::processKeyboard(GLFWwindow* window, GLfloat deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(front, up)) * movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(front, up)) * movementSpeed;

    updateCameraVectors();
}

void Camera::processMouseMovement(GLfloat xpos, GLfloat ypos, GLboolean constrainPitch)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float offsetx = xpos - lastX;
    float offsety = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    offsetx *= mouseSensitivity;
    offsety *= mouseSensitivity;

    pitch += offsety;
    yaw += offsetx;

    updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yOffset)
{
    zoom -= yOffset;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio)
{
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}

void Camera::updateCameraVectors()
{
    // Calcula o novo vetor Front
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Calcula o novo vetor Right e Up
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
