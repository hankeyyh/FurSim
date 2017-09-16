#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <glad\glad.h>

class WindField
{
public:
	WindField();
	virtual ~WindField();
	virtual glm::vec3 GetForce(glm::vec3 pos) = 0;
};


class WindField_Fan : public WindField
{
public:
	WindField_Fan(glm::vec3 _pos, glm::vec3 _dir, float _intensity, float _maxAngle, float _angleDamping, float _disDamping);
	~WindField_Fan() override;
	glm::vec3 GetForce(glm::vec3 pos) override;
	void SetStartPos(glm::vec3 _pos);
	void SetIntensity(float _intensity);
	void SetMaxAngle(float _maxAngle);

public:
	glm::vec3 startPos;
	glm::vec3 dir;
	float intensity;
	float maxAngle;
	float angleDamping;
	float distanceDamping;
};

class WindField_Disk : public WindField
{
public:
	WindField_Disk(glm::vec3 _point, glm::vec3 _dir, float _radius, float _intensity, float _disDamping);
	~WindField_Disk() override;

	glm::vec3 GetForce(glm::vec3 pos) override;
	void SetIntensity(float _intensity);
	void SetDirection(glm::vec3 _dir);
	void SetPointInPlane(glm::vec3 _point);
	void SetRadius(float _radius);
	void SetDisDamping(float _disDamping);
	glm::vec3 GetDirection() { return dir; }

public:
	glm::vec3 dir;
	glm::vec3 pointInPlane;
	float radius;
	float intensity;
	float distanceDamping;
};


class WindFieldRenderer
{
public:
	WindFieldRenderer(WindField_Disk* windDisk);
	~WindFieldRenderer();
	void SetUpGLMesh();
	void DeleteGLMesh();
	void Draw();

public:
	struct
	{
		GLuint VAO;
		GLuint VBO;
	};

	std::vector<glm::vec3> position;
};