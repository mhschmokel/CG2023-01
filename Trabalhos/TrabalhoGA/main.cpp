#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Common/include/Shader.h"

#include "Mesh.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 v_color;
};

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Protótipos das funções
int setupGeometry();
int loadSimpleObj(string filePath, int& nVertices, glm::vec3 color = glm::vec3(1.0, 0.0, 0.0));

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;

bool rotateX = false, rotateY = false, rotateZ = false;

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraSpeed = 0.05;

bool firstMouse = true;
float lastX = 0.0, lastY = 0.0;
float yaw = -90.0, pitch = 0.0;

bool increaseScale = false, decreaseScale = false;
bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false, moveFront = false, moveBack = false;
bool resetObject = false;

bool changeObject = false;

bool displayAllObjects = false;
bool spaceObjects = true;

int main()
{
	std::vector<std::string> modelsPath = {"../models/Cube/cube.obj", 
		"../models/Suzanne/suzanneTri.obj", 
		"../models/Pokemon/Pikachu.obj", 
		"../models/Classic-NoTexture/apple.obj" };

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Visualizador 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	//Desabilita o desenho do cursor do mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("../Common/shaders/camera.vs", "../Common/shaders/camera.fs");

	glUseProgram(shader.ID);

	glm::mat4 model = glm::mat4(1);
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	
	model = glm::rotate(model, (GLfloat)glfwGetTime()/*glm::radians(90.0f)*/, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	//Definindo a matriz de view (posição e orientação da câmera)
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

	//Definindo a matriz de projeção perpectiva
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	//Definindo as propriedades do material 
	shader.setFloat("ka", 0.4);
	shader.setFloat("kd", 0.5);
	shader.setFloat("ks", 0.5);
	shader.setFloat("q", 10);

	//Definindo as propriedades da fonte de luz
	shader.setVec3("lightPos", -2.0f, 10.0f, 3.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	std::vector<Mesh> objects;
	int currentObjectIdx = 0;

	Mesh object;
	GLuint VAO;
	int nVertices = 0;

	for (const std::string& path : modelsPath) {
		VAO = loadSimpleObj(path, nVertices);
		object.initialize(VAO, nVertices, &shader);
		objects.push_back(object);
	}

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Alterando a matriz de view (posição e orientação da câmera)
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

		//Enviando a posição da camera para o shader
		shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

		float angle = (GLfloat)glfwGetTime() * 10;

		if (displayAllObjects) {

			glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
			for (Mesh obj : objects) {
				obj.translate(pos);
				pos.x += 3.0;
				obj.draw();
			}
		}
		else {
			if (changeObject) {
				rotateX = false;
				rotateY = false;
				rotateZ = false;
				objects[currentObjectIdx].reset();
				currentObjectIdx++;
				currentObjectIdx = currentObjectIdx % objects.size();
				changeObject = false;
			}

			if (increaseScale) {
				objects[currentObjectIdx].increaseScale();
				increaseScale = false;
			}
			else if (decreaseScale) {
				objects[currentObjectIdx].decreaseScale();
				decreaseScale = false;
			}

			if (rotateX)
			{
				objects[currentObjectIdx].rotateX(angle);
			}
			else if (rotateY)
			{
				objects[currentObjectIdx].rotateY(angle);
			}
			else if (rotateZ)
			{
				objects[currentObjectIdx].rotateZ(angle);
			}

			if (moveUp) {
				objects[currentObjectIdx].moveUp();
				moveUp = false;
			}
			else if (moveDown) {
				objects[currentObjectIdx].moveDown();
				moveDown = false;
			}
			else if (moveRight) {
				objects[currentObjectIdx].moveRight();
				moveRight = false;
			}
			else if (moveLeft) {
				objects[currentObjectIdx].moveLeft();
				moveLeft = false;
			}
			else if (moveFront) {
				objects[currentObjectIdx].moveFront();
				moveFront = false;
			}
			else if (moveBack) {
				objects[currentObjectIdx].moveBack();
				moveBack = false;
			}

			if (resetObject) {
				rotateX = false;
				rotateY = false;
				rotateZ = false;
				objects[currentObjectIdx].reset();
				resetObject = false;
			}

			objects[currentObjectIdx].draw();
		}

		glfwSwapBuffers(window);
	}
	
	for (Mesh obj : objects) {
		obj.deleteVAO();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = !rotateX;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = !rotateY;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = !rotateZ;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		resetObject = true;
	}

	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	
	if (key == GLFW_KEY_KP_ADD) {
		increaseScale = true;
	}
	if (key == GLFW_KEY_KP_SUBTRACT) {
		decreaseScale = true;
	}

	if (key == GLFW_KEY_DOWN) {
		moveDown = true;
	}
	if (key == GLFW_KEY_UP) {
		moveUp = true;
	}
	if (key == GLFW_KEY_LEFT) {
		moveLeft = true;
	}
	if (key == GLFW_KEY_RIGHT) {
		moveRight = true;
	}
	if (key == GLFW_KEY_LEFT_BRACKET) {
		moveFront = true;
	}
	if (key == GLFW_KEY_RIGHT_BRACKET) {
		moveBack = true;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		changeObject = true;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		displayAllObjects = !displayAllObjects;
		resetObject = true;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

	offsetx *= cameraSpeed;
	offsety *= cameraSpeed;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

int loadSimpleObj(string filePath, int& nVertices, glm::vec3 color)
{
	ifstream inputFile;
	inputFile.open(filePath);
	vector <GLfloat> vertbuffer;

	vector <Vertex> vertices;
	vector <int> indices;
	vector <glm::vec3> normals;
	vector <glm::vec2> texCoord;

	if (inputFile.is_open())
	{
		char line[100];
		string sline;


		while (!inputFile.eof())
		{
			inputFile.getline(line, 100);
			sline = line;

			string word;
			istringstream ssline(sline);

			ssline >> word;

			if (word == "v")
			{
				Vertex v;
				ssline >> v.position.x >> v.position.y >> v.position.z;
				v.v_color.r = color.r; v.v_color.g = color.g; v.v_color.b = color.b;
				vertices.push_back(v);
			}
			if (word == "vt")
			{
				glm::vec2 vt;
				ssline >> vt.s >> vt.t;
				texCoord.push_back(vt);
			}
			if (word == "vn")
			{
				glm::vec3 vn;
				ssline >> vn.x >> vn.y >> vn.z;
				normals.push_back(vn);
			}
			else if (word == "f")
			{
				string tokens[3];
				for (int i = 0; i < 3; i++)
				{
					ssline >> tokens[i];
					int pos = tokens[i].find("/");
					string token = tokens[i].substr(0, pos);
					int index = atoi(token.c_str()) - 1;
					indices.push_back(index);
					vertbuffer.push_back(vertices[index].position.x);
					vertbuffer.push_back(vertices[index].position.y);
					vertbuffer.push_back(vertices[index].position.z);
					vertbuffer.push_back(vertices[index].v_color.r);
					vertbuffer.push_back(vertices[index].v_color.g);
					vertbuffer.push_back(vertices[index].v_color.b);

					tokens[i] = tokens[i].substr(pos + 1);
					pos = tokens[i].find("/");

					if (pos == 0) {
						tokens[i] = tokens[i].substr(pos + 1);
					}

					pos = tokens[i].find("/");
					token = tokens[i].substr(0, pos);
					int indexT = atoi(token.c_str()) - 1;

					if (indexT < 0) {
						indexT = 0;
					}

					if (texCoord.size() == 0) {
						vertbuffer.push_back(0);
						vertbuffer.push_back(0);
					}
					else {
						vertbuffer.push_back(texCoord[indexT].s);
						vertbuffer.push_back(texCoord[indexT].t);
					}

					tokens[i] = tokens[i].substr(pos + 1);
					token = tokens[i].substr(0, pos);
					int indexN = atoi(token.c_str()) - 1;

					if (indexN < 0) {
						indexN = 0;
					}

					vertbuffer.push_back(normals[indexN].x);
					vertbuffer.push_back(normals[indexN].y);
					vertbuffer.push_back(normals[indexN].z);

				}

			}

		}

		inputFile.close();
	}
	else
	{
		cout << "Não foi possivel abrir o arquivo " << filePath << endl;
	}


	nVertices = vertbuffer.size() / 11;

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertbuffer.size() * sizeof(GLfloat), vertbuffer.data(), GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenadas de textura (s, t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Atributo vetor normal (x, y e z)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}

