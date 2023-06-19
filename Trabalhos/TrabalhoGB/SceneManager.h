#pragma once
#include "../Common/include/Shader.h"
#include "SceneStruct.cpp"
#include "Object.h"
#include "objectOperationsEnum.cpp"

class SceneManager
{
public:
	SceneManager() {}
	~SceneManager() {}
	void init(string iniPath, Shader *shader);
	void draw();
	void selectNextObject();
	void objectOperation(ObjOperationsEnum operation);

protected:
	sceneStruct sceneStruct = {};
	vector <Object> objects;

	int currentObjectIndex = 0;
	bool displayOnlySelectedObj = false;
};

