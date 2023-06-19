#include "../Common/include/Shader.h"
#include <vector>
#include "objectStruct.cpp"
#include "lightStruct.cpp"
#include "cameraStruct.cpp"

struct sceneStruct {
	vector <objectStruct> objects;
	lightStruct light;
	cameraStruct camera;
};