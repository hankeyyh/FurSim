
#include "Vertex.h"

float Vertex::L0;

Vertex::Vertex(vec3 pos, float m) : Position(pos), tmp_Position(pos)
{
	if (m < 0.001)
		m = 0.001;

	Mass = m;
}

