#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Common/include/Shader.h"

class Mesh
{
public:
	Mesh() {}
	~Mesh() {}
	void initialize(GLuint VAO, int nVertices, Shader* shader, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1), float angle = 0.0, glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0));
	void update();
	void draw();
	void translate(glm::vec3 position);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void increaseScale();
	void decreaseScale();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void moveFront();
	void moveBack();
	void reset();
	void deleteVAO();

protected:
	GLuint VAO;
	int nVertices;
	glm::vec3 position;
	glm::vec3 originalPosition;
	float angle;
	glm::vec3 axis;
	glm::vec3 scale;

	float movementSpeed = 0.05;

	Shader* shader;

};

