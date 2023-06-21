#include "Curve.h"
#include <vector>

Curve::Curve()
{
	M = glm::mat4(-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0
	);

}

void readControlPoints(string filename, float scale) {
    vector<vec3*> curvePoints;

    ifstream inputFile;
    inputFile.open(filename.c_str());
    
    if (inputFile.is_open())
    {
        string line;
        string identifier;

    while (!inputFile.eof()) {
        
        getline(inputFile, line);
        
        stringstream sline(line);

        sline >> identifier;

        if (identifier.empty()) {
            continue;
        }

        if (identifier == "v") {
            float x, y, z;
            sline >> x >> y >> z;
            curvePoints.push_back(new vec3(x * scale, y * scale, z * scale));
        }
    }

    inputFile.close();

	this->controlPoints = curvePoints;

}

GLuint generateVAO(vector<vec3*> controlPoints)
{
		GLuint VBO, VAO;

		//Geração do identificador do VBO
		glGenBuffers(1, &VBO);

		//Faz a conexão (vincula) do buffer como um buffer de array
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//Envia os dados do array de floats para o buffer da OpenGl
		glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(GLfloat) * 3, controlPoints.data(), GL_STATIC_DRAW);

		//Geração do identificador do VAO (Vertex Array Object)
		glGenVertexArrays(1, &VAO);

		// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
		// e os ponteiros para os atributos 
		glBindVertexArray(VAO);

		//Atributo posição (x, y, z)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
		// atualmente vinculado - para que depois possamos desvincular com segurança
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
		glBindVertexArray(0);

		return VAO;
}

void Curve::setShader(Shader* shader)
{
	this->shader = shader;
	shader->Use();
}

glm::vec3 Curve::getPointOnCurve(int point)
{
	glm::vec3 point = curvePoints.at(index);
	return point;
}

int Curve::getNumberCurvePoints()
{
	return numberCurvePoints;
}

void Curve::generateCurve(int pointsPerSegment)
{
	this->numberCurvePoints = pointsPerSegment;

	float step = 1.0 / (float)pointsPerSegment;

	float t = 0;

	int nControlPoints = controlPoints.size();

	for (int i = 0; i < nControlPoints - 3; i += 3)
	{

		for (float t = 0.0; t <= 1.0; t += step)
		{
			glm::vec3 p;

			glm::vec4 T(t * t * t, t * t, t, 1);

			glm::vec3 P0 = controlPoints[i];
			glm::vec3 P1 = controlPoints[i + 1];
			glm::vec3 P2 = controlPoints[i + 2];
			glm::vec3 P3 = controlPoints[i + 3];

			glm::mat4x3 G(P0, P1, P2, P3);

			p = G * M * T;

			curvePoints.push_back(p);
		}
	}

	//Gera o VAO
	GLuint VBO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(GLfloat) * 3, curvePoints.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
}

