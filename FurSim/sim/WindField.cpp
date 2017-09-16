#include "WindField.h"
#include <iostream>


WindField::WindField()
{
}


WindField::~WindField()
{
}

WindField_Fan::WindField_Fan(glm::vec3 _pos, glm::vec3 _dir, float _intensity, float _maxAngle, float _angleDamping, float _disDamping) : WindField(),
startPos(_pos),
dir(_dir),
intensity(_intensity),
maxAngle(_maxAngle),
angleDamping(_angleDamping),
distanceDamping(_disDamping)
{

}

WindField_Fan::~WindField_Fan()
{

}

glm::vec3 WindField_Fan::GetForce(glm::vec3 pos)
{
	glm::vec3 windDir = glm::normalize(pos - startPos);
	dir = glm::normalize(dir);

	float distance = glm::length(pos - startPos);
	float cosAngle = glm::dot(windDir, dir);
	float angle = glm::acos(cosAngle);

	float tmpIntensity = -distanceDamping * distance + intensity;
	if (tmpIntensity < FLT_MIN)
	{
		return glm::vec3(0);
	}
	tmpIntensity += (angle > maxAngle) ? 0 : intensity * cosAngle * angleDamping;
	if (tmpIntensity < FLT_MIN)
	{
		return glm::vec3(0);
	}
	return dir * tmpIntensity;
}

void WindField_Fan::SetStartPos(glm::vec3 _pos)
{
	this->startPos = _pos;
}

void WindField_Fan::SetIntensity(float _intensity)
{
	this->intensity = _intensity;
}

void WindField_Fan::SetMaxAngle(float _maxAngle)
{
	this->maxAngle = _maxAngle;
}


//WindField Plane
WindField_Disk::WindField_Disk(glm::vec3 _point, glm::vec3 _dir, float _radius, float _intensity, float _disDamping) : WindField(),
pointInPlane(_point),
dir(_dir),
radius(_radius),
intensity(_intensity),
distanceDamping(_disDamping)
{

}

WindField_Disk::~WindField_Disk()
{

}

glm::vec3 WindField_Disk::GetForce(glm::vec3 pos)
{
	glm::vec3 posToPointDir = glm::normalize(pos - pointInPlane);
	float ang = glm::acos(glm::dot(posToPointDir, normalize(dir)));
	float disOnPlane = glm::length(pos - pointInPlane) * glm::sin(ang);
	if (disOnPlane > radius)
		return glm::vec3(0);

	float distance = glm::dot((pos - pointInPlane), dir) / glm::length(dir);
	float tmp_Intensity = -distanceDamping * distance + intensity;

	if (tmp_Intensity < FLT_MIN)
		tmp_Intensity = 0;
	
	return glm::normalize(dir) * tmp_Intensity;
}

void WindField_Disk::SetIntensity(float _intensity)
{
	this->intensity = _intensity;
}

void WindField_Disk::SetDirection(glm::vec3 _dir)
{
	this->dir = _dir;
}

void WindField_Disk::SetPointInPlane(glm::vec3 _point)
{
	this->pointInPlane = _point;
}

void WindField_Disk::SetRadius(float _radius)
{
	this->radius = _radius;
}

void WindField_Disk::SetDisDamping(float _disDamping)
{
	this->distanceDamping = _disDamping;
}

WindFieldRenderer::WindFieldRenderer(WindField_Disk* windDisk)
{
	float radius = windDisk->radius;
	glm::vec3 origin = windDisk->pointInPlane;

	
	float ang = 0;
	while (ang <= 2 * M_PI)
	{
		//glm::vec3 pos = origin + radius *
	}
}

WindFieldRenderer::~WindFieldRenderer()
{
	DeleteGLMesh();
}

void WindFieldRenderer::SetUpGLMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(1, position.size() * sizeof(glm::vec3), position.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void WindFieldRenderer::DeleteGLMesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}

void WindFieldRenderer::Draw()
{
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINES, 0, position.size());
	glBindVertexArray(0);
}