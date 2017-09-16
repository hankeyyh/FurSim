#pragma once
#include <glad\glad.h>

class Sphere
{
public:
	Sphere(int _lats, int _longs);
	~Sphere();
	void Init();
	void Draw();

public:
	unsigned m_VAO;
	unsigned m_VBO;
	unsigned m_EBO;

	int lats, longs;
	int numsToDraw;
	bool isInited;
};

