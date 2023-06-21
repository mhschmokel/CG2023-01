#include "IniFileParser.h"
#include <vector>

sceneStruct IniFileParser::parse(string iniFilePath) {
	char line[150];
	string sline;

	sceneStruct sceneStruct = {};
	objectStruct objStruct = {};

	vector <objectStruct> objStructs;

	lightStruct lightS = {};
	cameraStruct cameraS = {};

	bool hasToAddNewObject = false;

	ifstream inputFile;
	inputFile.open(iniFilePath.c_str());

	if (inputFile.is_open()) {
		while (inputFile.good()) {
			inputFile.getline(line, 150);
		
			sline = line;

			string param;
			string value1, value2, value3;

			istringstream ssline(line);
			ssline >> param >> value1 >> value2 >> value3;

			if (param == "object") {
				objStruct.filePath = value1;
				hasToAddNewObject = true;
			}

			if (param == "rotation") {
				objStruct.angle = std::stof(value1);
			}

			if (param == "position") {
				glm::float32 v1 = glm::float32(std::stof(value1));
				glm::float32 v2 = glm::float32(std::stof(value2));
				glm::float32 v3 = glm::float32(std::stof(value3));
				objStruct.position = glm::vec3(v1, v2, v3);
			}

			if (param == "scale") {
				glm::float32 v1 = glm::float32(std::stof(value1));
				glm::float32 v2 = glm::float32(std::stof(value2));
				glm::float32 v3 = glm::float32(std::stof(value3));
				objStruct.scale = glm::vec3(v1, v2, v3);
			}

			if (sline.empty() && hasToAddNewObject) {
				objStructs.push_back(objStruct);
				hasToAddNewObject = false;
			}

			if (param == "lightPos") {
				glm::float32 v1 = glm::float32(std::stof(value1));
				glm::float32 v2 = glm::float32(std::stof(value2));
				glm::float32 v3 = glm::float32(std::stof(value3));
				lightS.position = glm::vec3(v1, v2, v3);
			}

			if (param == "lightColor") {
				glm::float32 v1 = glm::float32(std::stof(value1));
				glm::float32 v2 = glm::float32(std::stof(value2));
				glm::float32 v3 = glm::float32(std::stof(value3));
				lightS.color = glm::vec3(v1, v2, v3);
			}

			if (param == "lightSpecular") {
				lightS.specular = std::stoi(value1);
			}

			if (param == "cameraPosition") {
				glm::float32 v1 = glm::float32(std::stof(value1));
				glm::float32 v2 = glm::float32(std::stof(value2));
				glm::float32 v3 = glm::float32(std::stof(value3));
				cameraS.cameraPosition = glm::vec3(v1, v2, v3);
			}

			if (param == "cameraFront") {
				glm::float32 v1 = glm::float32(std::stof(value1));
				glm::float32 v2 = glm::float32(std::stof(value2));
				glm::float32 v3 = glm::float32(std::stof(value3));
				cameraS.cameraFront = glm::vec3(v1, v2, v3);
			}

			if (param == "cameraUp") {
				glm::float32 v1 = glm::float32(std::stof(value1));
				glm::float32 v2 = glm::float32(std::stof(value2));
				glm::float32 v3 = glm::float32(std::stof(value3));
				cameraS.cameraUp = glm::vec3(v1, v2, v3);
			}

			if (param == "cameraSpeed") {
				cameraS.cameraSpeed = std::stof(value1);
			}

			if (param == "curvePoints") {
				objStruct.meshPath = value1;
			}

			if (inputFile.eof()) {
				sceneStruct.objects = objStructs;
				sceneStruct.light = lightS;
				sceneStruct.camera = cameraS;
			}

		}
		inputFile.close();
	}
	else {
		cout << "Unable to open file";
	}

	return sceneStruct;
}