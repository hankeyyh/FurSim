#include "Helper.h"
#include <math.h>

glm::vec3 GetPoint(float u, float v)
{
	float x = sin(M_PI * v) * cos(2 * M_PI * u);
	float y = sin(M_PI * v) * sin(2 * M_PI * u);
	float z = cos(M_PI * v);

	return glm::vec3(x, y, z);
}