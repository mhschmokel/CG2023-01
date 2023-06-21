#pragma once

#include <vector>
#include <stb_image.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Common/include/Shader.h"
#include "Mesh.h"
#include "mtlStruct.cpp"

class Object
{
public:
	Object() {}
	~Object() {}
	void initialize(string filePath, 
					Shader* shader, 
					glm::vec3 position = glm::vec3(0.0f), 
					glm::vec3 scale = glm::vec3(1), 
					float angle = 0.0, 
					glm::vec3 axis = glm::vec3(0.0, 1.0, 0.0));
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
	void loadObj(string filePath);
	GLuint generateVAO(vector<GLfloat> vertbuffer, int& nVerts);
	GLuint loadTexture(string filePath);
	string getTexturePath(string mtllibFilePath, string newmtlId);
	vector <mtl> getObjectMtl(string mtllibFilePath);

	GLuint VAO;
	int nVertices;
	glm::vec3 position;
	glm::vec3 originalPosition;
	float angle;
	glm::vec3 axis;
	glm::vec3 scale;
	GLuint texID;

	float movementSpeed = 0.05;

	Shader* shader;
	vector <Mesh> meshes;

};

