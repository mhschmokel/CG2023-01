#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = -90.0f, GLfloat pitch = 0.0f);

    void processKeyboard(GLFWwindow* window, GLfloat deltaTime);
    void processMouseMovement(GLfloat xpos, GLfloat ypos, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat yOffset);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float aspectRatio);

private:
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw = -90.0;
    GLfloat pitch;

    GLfloat movementSpeed = 0.05;
    GLfloat mouseSensitivity = 0.05;
    GLfloat zoom = 45.0;

    bool firstMouse = true;
    GLfloat lastX = 0.0;
    GLfloat lastY = 0.0;
};
