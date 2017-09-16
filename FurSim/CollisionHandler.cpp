#include "CollisionHandler.h"
#include <iostream>


CollisionHandler::CollisionHandler()
{
	
}


CollisionHandler::~CollisionHandler()
{
}

bool CollisionHandler::IsColliding(Vertex& ballA, Vertex& ballB, float radiusA, float radiusB)
{
	glm::vec3 distance = ballA.Position - ballB.Position;

	if (glm::length(distance) <= radiusA + radiusB)
	{
		return true;
	}
	return false;
}

void CollisionHandler::HandleCollision(Vertex& ballA, Vertex& ballB, float radiusA, float radiusB, float ks)
{
	//get mtd
	glm::vec3 delta = ballA.Position - ballB.Position;

	// minimum translation distance to push balls apart after intersecting
	glm::vec3 mtd = delta * (((radiusA + radiusB) - glm::length(delta)) / glm::length(delta));

	float im1 = 1.0f / ballA.Mass;
	float im2 = /*1.0f / ballB.Mass*/0;


	ballA.Force += (1.0f / ks) * mtd;



	// push-pull them apart based off their mass
	//ballA.Position += mtd * (im1 / (im1 + im2));
	//ballB.Position -= mtd * (im2 / (im1 + im2));


	// impact speed
	//glm::vec3 vel = ballA.Velocity - ballB.Velocity;
	//float vn = glm::dot(vel, glm::normalize(mtd));

	//// sphere intersecting but moving away from each other already
	//if (vn > 0.0f)
	//	return;

	//// collision impulse
	//float restitution = 0.85f;
	//float i = (-(1.0f + restitution) * vn) / (im1 + im2);
	//glm::vec3 impulse = /*glm::normalize(mtd)*/mtd * i;

	//// change in momentum
	//ballA.Velocity += impulse * im1;
	//ballB.Velocity -= impulse * im2;
}

void CollisionHandler::Update(Hair& hair, Vertex& object, float objectRadius, float ks)
{
	//Vertex particle(glm::vec3(0), FLT_MAX);
	//particle.Velocity = glm::vec3(0);
	for (int s = 0; s < hair.size(); s++)
	{
		for (int v = 0; v < hair[s].size(); v++)
		{
			if (IsColliding(hair[s][v], object, Vertex::L0, objectRadius))
			{
				//头部按圆球计算碰撞
				HandleCollision(hair[s][v], object, Vertex::L0, objectRadius, ks);//点与点之间距离始终是0.06
			}
		}
	}
}