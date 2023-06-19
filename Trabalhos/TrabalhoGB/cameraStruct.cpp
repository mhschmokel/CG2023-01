#include <glm/glm.hpp>

struct cameraStruct {
	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float cameraSpeed;
};