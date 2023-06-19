#include "SceneManager.h"
#include "IniFileParser.h"

void SceneManager::init(string iniPath, Shader *shader) {
	IniFileParser iniFileParser;
	this->sceneStruct = iniFileParser.parse(iniPath);
	objects = {};

	for (int i = 0; i < sceneStruct.objects.size(); i++) {
		Object obj;
		obj.initialize(sceneStruct.objects[i].filePath,
					   shader,
					   sceneStruct.objects[i].position,
					   sceneStruct.objects[i].scale,
					   sceneStruct.objects[i].angle);
		objects.push_back(obj);
	}

	this->loaded = true;
}

bool SceneManager::isLoaded() {
	return this->loaded;
}

lightStruct SceneManager::getLightStruct()
{
	return sceneStruct.light;
}

cameraStruct SceneManager::getCameraStruct()
{
	return sceneStruct.camera;
}

void SceneManager::draw() {
	if (displayOnlySelectedObj) {
		objects[currentObjectIndex].draw();
	}
	else {
		for (int i = 0; i < objects.size(); i++) {
			objects[i].draw();
		}
	}
	
}

void SceneManager::selectNextObject() {
	currentObjectIndex++;
	currentObjectIndex = currentObjectIndex % objects.size();
}

void SceneManager::objectOperation(ObjOperationsEnum operation)
{
	float angle = (GLfloat)glfwGetTime() * 10;
	switch (operation) {
	case mUp:
		objects[currentObjectIndex].moveUp();
		break;
	case mDown:
		objects[currentObjectIndex].moveDown();
		break;
	case mRight:
		objects[currentObjectIndex].moveRight();
		break;
	case mLeft:
		objects[currentObjectIndex].moveLeft();
		break;
	case mFront:
		objects[currentObjectIndex].moveFront();
		break;
	case mBack:
		objects[currentObjectIndex].moveBack();
		break;
	case incScale:
		objects[currentObjectIndex].increaseScale();
		break;
	case decScale:
		objects[currentObjectIndex].decreaseScale();
		break;
	case rotX:
		objects[currentObjectIndex].rotateX(angle);
		break;
	case rotY:
		objects[currentObjectIndex].rotateY(angle);
		break;
	case rotZ:
		objects[currentObjectIndex].rotateZ(angle);
		break;
	case displayOnlySelected:
		displayOnlySelectedObj = !displayOnlySelectedObj;
		break;
	case resetValues:
		objects[currentObjectIndex].reset();
		break;
	case None:
		break;
	}

}