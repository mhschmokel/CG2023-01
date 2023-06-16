#include "Mesh.h"

void Mesh::initialize(GLuint VAO, int nVertices, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
{
	this->VAO = VAO;
	this->nVertices = nVertices;
	this->shader = shader;
	this->position = position;
	this->originalPosition = position;
	this->scale = scale;
	this->angle = angle;
	this->axis = axis;

}

void Mesh::update()
{
	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), axis);
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
}

void Mesh::rotateX(float angle) {
	this->angle = angle;
	this->axis = glm::vec3(1.0f, 0.0f, 0.0f);
	update();
}

void Mesh::rotateY(float angle) {
	this->angle = angle;
	this->axis = glm::vec3(0.0f, 1.0f, 0.0f);
	update();
}

void Mesh::rotateZ(float angle) {
	this->angle = angle;
	this->axis = glm::vec3(0.0f, 0.0f, 1.0f);
	update();
}

void Mesh::translate(glm::vec3 position)
{
	this->position = position;
	update();
}

void Mesh::increaseScale() {
	scale.x += 0.05;
	scale.y += 0.05;
	scale.z += 0.05;
	update();
}

void Mesh::decreaseScale() {
	scale.x -= 0.05;
	scale.y -= 0.05;
	scale.z -= 0.05;
	update();
}

void Mesh::moveUp()
{
	this->position.y = this->position.y + movementSpeed;
	update();
}

void Mesh::moveDown()
{
	this->position.y = this->position.y - movementSpeed;
	update();
}

void Mesh::moveLeft()
{
	this->position.x = this->position.x - movementSpeed;
	update();
}

void Mesh::moveRight()
{
	this->position.x = this->position.x + movementSpeed;
	update();
}

void Mesh::moveFront()
{
	this->position.z = this->position.z + movementSpeed;
	update();
}

void Mesh::moveBack()
{
	this->position.z = this->position.z - movementSpeed;
	update();
}

void Mesh::reset() {
	this->position = originalPosition;
	this->scale = glm::vec3(1);
	this->angle = 0.0;
	update();
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, nVertices);
	glBindVertexArray(0);
}

void Mesh::deleteVAO() {
	glDeleteVertexArrays(1, &VAO);
}
