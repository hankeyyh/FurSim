#include "MassSpring.h"
#include <gl\glut.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

MassSpring::MassSpring(Hair& h, float _springConstant, float _length) : ISimulation(h),
springConstant(_springConstant),
maxLength(_length)
{
	this->airFriction = 0.003;
	this->inertia = (h[0][0].Mass * maxLength * maxLength) / 12.0f;

	for (size_t s = 0; s < this->hair.size(); s++)
	{
		const size_t nVert = this->hair[s].size();
		Vertex* restrict verts = this->hair[s].data();

		for (size_t v = 0; v < nVert; v++) //include the very first vertex
		{
			verts[v].OldPosition = verts[v].Position;
		}
	}
}


MassSpring::~MassSpring()
{

}

int lastTime = 0;
float slowMotionRatio = 1.0f;

void MassSpring::update()
{
	int timeNow;
	timeNow = glutGet(GLUT_ELAPSED_TIME);
	int milliseconds = timeNow - lastTime;
	lastTime = timeNow;
	float dt = milliseconds / 1000.0f;  // konvertera till millisekunder
	float maxPossible_dt = 0.001f;		
										
	dt /= slowMotionRatio;

	dt = 1.0f / 100.0f;

	int numOfIterations = (int)(dt / maxPossible_dt) + 1;		
	if (numOfIterations != 0)									
		dt = dt / numOfIterations;

	for (int a = 0; a < 10; ++a)
	{
		ResetForce();
		Solve();
		Simulate(dt);
	}
}


void MassSpring::ResetForce()
{
	for (size_t s = 0; s < this->hair.size(); s++)
	{
		const size_t nVert = this->hair[s].size();
		Vertex* restrict verts = this->hair[s].data();

		for (size_t v = 1; v < nVert; v++) //skip the very first vertex
		{
			verts[v].Force = glm::vec3(0, 0, 0);
		}
	}
}

void MassSpring::Solve()
{
	for (size_t s = 0; s < this->hair.size(); s++)
	{
		const size_t nVert = this->hair[s].size();
		Vertex* restrict verts = this->hair[s].data();

		for (size_t v = 0; v < nVert; v++) //skip the very first vertex
		{
			glm::vec3 springForce;
			glm::vec3 springVector;
			glm::vec3 force;

			if (v == 0)
			{
				springVector = -glm::vec3(0, Vertex::L0, 0);
				springForce += -verts[v].Velocity * inertia;
				length = springVector.length();
				springForce += (springVector / length) * ((length - maxLength) * springConstant);
				//std::cout << "springForce: " << springForce.x << " " << springForce.y << " " << springForce.z << std::endl;
				verts[v].Force += springForce;
			}
			else
			{
				springVector = verts[v - 1].OldPosition - verts[v].Position;
				springForce += (verts[v - 1].Velocity - verts[v].Velocity) * inertia;
				float length = springVector.length();
				springForce += (springVector / length) * ((length - maxLength) * springConstant);
				//std::cout << "springForce: " << springForce.x << " " << springForce.y << " " << springForce.z << std::endl;
				verts[v].Force += springForce;
				verts[v - 1].Force += -springForce;
			}

			glm::vec3 gravity(0.0, -0.3f, 0.0);
			force += gravity;

			glm::vec3 airForce;
			airForce = -verts[v].Velocity * airFriction;
			force += airForce;
			verts[v].Force += force;

			srand(time(0));
			int var = rand() % 10; // slumptal mellan noll och tio
			glm::vec3 wind = glm::vec3(-(float)(var / 250.0), 0.0, 0.0);
			verts[v].Force += wind;
		}
	}
}

void MassSpring::Simulate(float dt)
{
	constexpr float TimeStep = 1.0f / 100.0f; // mentioned as dt

	for (size_t s = 0; s < this->hair.size(); s++)
	{
		const size_t nVert = this->hair[s].size();
		Vertex* restrict verts = this->hair[s].data();

		for (size_t v = 1; v < nVert; v++) //skip the very first vertex
		{
			verts[v].OldPosition = verts[v].Position;
			verts[v].Velocity += (verts[v].Force / verts[v].Mass) * dt;
			verts[v].Position += verts[v].Velocity * dt;
		}
	}
}
