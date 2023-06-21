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
#include "SceneManager.h"
#include "objectOperationsEnum.cpp"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 v_color;
};

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void setCallbacks(GLFWwindow* window);
void initIMGUI(GLFWwindow* window);
void initCameraAndLight(Shader shader);
void displayIMGUI(Shader *shader);
void updateCamera(Shader shader, GLFWwindow* window);
void closeIMGUI();

SceneManager sceneManager;

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;
const char* glsl_version = "#version 450";

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;
float cameraSpeed;

bool firstMouse = true;
float lastX = 0.0, lastY = 0.0;
float yaw = -90.0, pitch = 0.0;

bool changeObject = false;

bool hasToDisableCursor = true;
bool hasToInitCameraAndLight = true;

ObjOperationsEnum objOperation = None;



int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Visualizador 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	setCallbacks(window);
	initIMGUI(window);

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

	//Definindo a matriz de projeção perpectiva
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.ID, "colorBuffer"), 0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		displayIMGUI(&shader);
		initCameraAndLight(shader);
		updateCamera(shader, window);

		sceneManager.objectOperation(objOperation);
		sceneManager.draw();

		if (changeObject) {
			sceneManager.selectNextObject();
			changeObject = false;
		}

		if (objOperation == rotX || objOperation == rotY || objOperation == rotZ) {
			//do nothing to keep object rotating
		}
		else {
			objOperation = None;
		}

		glfwSwapBuffers(window);
	}
	
	closeIMGUI();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if (objOperation == rotX) {
			objOperation = None;
		}
		else {
			objOperation = rotX;
		}
		
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		if (objOperation == rotY) {
			objOperation = None;
		}
		else {
			objOperation = rotY;
		}
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (objOperation == rotZ) {
			objOperation = None;
		}
		else {
			objOperation = rotZ;
		}
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		objOperation = resetValues;
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
	
	if (key == GLFW_KEY_PAGE_UP) {
		objOperation = incScale;
	}
	if (key == GLFW_KEY_PAGE_DOWN) {
		objOperation = decScale;
	}

	if (key == GLFW_KEY_DOWN) {
		objOperation = mDown;
	}
	if (key == GLFW_KEY_UP) {
		objOperation = mUp;
	}
	if (key == GLFW_KEY_LEFT) {
		objOperation = mLeft;
	}
	if (key == GLFW_KEY_RIGHT) {
		objOperation = mRight;
	}
	if (key == GLFW_KEY_LEFT_BRACKET) {
		objOperation = mBack;
	}
	if (key == GLFW_KEY_RIGHT_BRACKET) {
		objOperation = mFront;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		changeObject = true;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		objOperation = displayOnlySelected;
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

void setCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void initIMGUI(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void initCameraAndLight(Shader shader)
{
	if (sceneManager.isLoaded() && hasToInitCameraAndLight) {
		//Definindo a matriz de view (posição e orientação da câmera)
		cameraStruct cameraS = sceneManager.getCameraStruct();

		cameraPos = cameraS.cameraPosition;
		cameraFront = cameraS.cameraFront;
		cameraUp = cameraS.cameraUp;
		cameraSpeed = cameraS.cameraSpeed;

		glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0), cameraUp);
		GLint viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

		lightStruct lightS = sceneManager.getLightStruct();
		shader.setFloat("q", lightS.specular);
		//Definindo as propriedades da fonte de luz
		shader.setVec3("lightPos", lightS.position.x, lightS.position.y, lightS.position.z);
		shader.setVec3("lightColor", lightS.color.x, lightS.color.y, lightS.color.z);

		hasToInitCameraAndLight = false;
	}
}

void displayIMGUI(Shader *shader)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// ImGui toolbox window
	ImGui::Begin("Toolbox");

	// ImGui UI elements, for example:
	ImGui::Text("Model Import");
	if (ImGui::Button("Load .ini"))
	{
		// Load your .ini file here
		ImGuiFileDialog::Instance()->OpenDialog("ChooseIniFile", "Choose .ini file", ".ini\0", ".");
	}

	if (ImGuiFileDialog::Instance()->Display("ChooseIniFile"))
	{
		// Check if the user selected a file
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// Get the selected file path
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			sceneManager.init(filePath, shader);
		}

		// Close the ImGuiFileDialog
		ImGuiFileDialog::Instance()->Close();
	}


	ImGui::End();

	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void updateCamera(Shader shader, GLFWwindow* window)
{
	if (hasToDisableCursor) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		GLint viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));
		shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void closeIMGUI() 
{
	ImGuiFileDialog::Instance()->Close();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}