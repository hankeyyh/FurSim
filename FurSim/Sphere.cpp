#include "Sphere.h"
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>



Sphere::Sphere(int _lats, int _longs) :
	lats(_lats),
	longs(_longs),
	isInited(false)
{
}


Sphere::~Sphere()
{
	if (!isInited)
		return;

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Sphere::Init()
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	int indicator = 0;
	for (int i = 0; i <= lats; i++)
	{
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (int j = 0; j <= longs; j++)
		{
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			vertices.push_back(x * zr0);
			vertices.push_back(y * zr0);
			vertices.push_back(z0);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x * zr1);
			vertices.push_back(y * zr1);
			vertices.push_back(z1);
			indices.push_back(indicator);
			indicator++;
		}
		//indices.push_back(GL_PRIMITIVE_RESTART_INDEX);//文章用的是GL_PRIMITIVE_RESTART_FIXED_INDEX
	}

	// 创建并绑定环境
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(m_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	}
	glBindVertexArray(0);

	numsToDraw = indices.size();
	isInited = true;
}

void Sphere::Draw()
{
	if (!isInited)
	{
		std::cout << "please call init() before draw()" << std::endl;
		return;
	}

	// draw sphere
	glBindVertexArray(m_VAO);
	//glEnable(GL_PRIMITIVE_RESTART);
	//glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glDrawElements(GL_TRIANGLE_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);
}