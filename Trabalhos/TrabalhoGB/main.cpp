#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stb_image.h>

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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ImGuiFileDialog.h"

#include "Object.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 v_color;
};

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Protótipos das funções
int setupGeometry();
int loadSimpleObj(string filePath, int& nVertices, glm::vec3 color = glm::vec3(1.0, 0.0, 0.0));
GLuint generateTexture(string filepath);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;
const char* glsl_version = "#version 450";

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

bool hasToDisableCursor = true;

int main()
{
	std::vector<std::string> modelsPath = {
		/*"../models/Cube/cube.obj", 
		"../models/Suzanne/suzanneTri.obj", 
		"../models/Pokemon/Pikachu.obj", 
		"../models/Classic-NoTexture/apple.obj" */
	};

	

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Visualizador 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	 

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

	Shader shader("../Common/shaders/camera_texture_phong.vs", "../Common/shaders/camera_texture_phong.fs");

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

	glEnable(GL_DEPTH_TEST);

	//Definindo as propriedades do material 
	shader.setFloat("ka", 0.4);
	shader.setFloat("kd", 0.5);
	shader.setFloat("ks", 0.5);
	shader.setFloat("q", 10);

	//Definindo as propriedades da fonte de luz
	shader.setVec3("lightPos", -2.0f, 10.0f, 3.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.ID, "colorBuffer"), 0);

	std::vector<Object> objects;
	int currentObjectIdx = 0;

	
	GLuint VAO;
	int nVertices = 0;

	/*for (const std::string& path : modelsPath) {
		VAO = loadSimpleObj(path, nVertices);
		object.initialize(VAO, nVertices, &shader);
		objects.push_back(object);
	}*/
	

	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		float angle = (GLfloat)glfwGetTime() * 10;

		if (hasToDisableCursor) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			//Alterando a matriz de view (posição e orientação da câmera)
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

			//Enviando a posição da camera para o shader
			shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (objects.size() > 0) {
			if (displayAllObjects) {

				glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
				for (Object obj : objects) {
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
		}

		// ImGui toolbox window
		ImGui::Begin("Toolbox");

		// ImGui UI elements, for example:
		ImGui::Text("Model Import");
		if (ImGui::Button("Load .obj"))
		{
			// Load your .obj file here
			ImGuiFileDialog::Instance()->OpenDialog("ChooseObjFile", "Choose .obj file", ".obj\0", ".");
		}

		if (ImGuiFileDialog::Instance()->Display("ChooseObjFile"))
		{
			// Check if the user selected a file
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				// Get the selected file path
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				cout << "File path: " << filePath << endl;
				// Load the .obj file using your existing loading function
				/*VAO = loadSimpleObj(filePath, nVertices);*/
				Object object;
				
				object.initialize(filePath, &shader);
				objects.push_back(object);

				currentObjectIdx = objects.size() - 1;
			}

			// Close the ImGuiFileDialog
			ImGuiFileDialog::Instance()->Close();
		}


		ImGui::End();

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	
	for (Object obj : objects) {
		obj.deleteVAO();
	}
	ImGuiFileDialog::Instance()->Close();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse)
	{
		// ImGui wants to capture the mouse, so don't process the clicks for your 3D scene
		return;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// Left mouse button was pressed, do something here
		hasToDisableCursor = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		// Right mouse button was pressed, do something here
		hasToDisableCursor = false;
	}
}