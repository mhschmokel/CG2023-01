#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace glm;

#define PI 3.14159265359
#define HALF_PI PI/2.0

class Curve
{
public:
	Curve() {}

	void readControlPoints(string filename, float scale);
	void setShader(Shader* shader);
	void generateCurve(int pointsPerSegment);
	GLuint generateVAO(vector<vec3*> controlPoints);
	glm::vec3 getPointOnCurve(int point);
	int getNumberCurvePoints();

private:
	string path;
	float scale;
	vector<vec3*> controlPoints;
	vector<vec3> curvePoints;
	int numberCurvePoints;
};