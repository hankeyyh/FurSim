#include "MassSpring_Mine.h"



MassSpring_Mine::MassSpring_Mine(Hair& h, float _stiffness, float _damping) : ISimulation(h),
	stiffness(_stiffness),
	damping(_damping)
{
	
}


MassSpring_Mine::~MassSpring_Mine()
{
}

void MassSpring_Mine::update()
{
	float dt = 1.0f / 100.0f;

	for (size_t s = 0; s < this->hair.size(); s++)
	{
		const size_t nVert = this->hair[s].size();
		Vertex* restrict verts = this->hair[s].data();

		std::vector<glm::vec3> springForces;
		std::vector<glm::vec3> dampingForces;
		springForces.resize(nVert);
		dampingForces.resize(nVert);

		//计算弹力/阻力
		for (size_t v = 0; v < nVert; v++)
		{
			if (!verts[v].enabled)
			{
				springForces[v] = glm::vec3(0);
				dampingForces[v] = glm::vec3(0);
			}
			else
			{
				springForces[v] = -stiffness * (verts[v].Position - /*verts[v].OldPosition*/verts[v - 1].Position);
				dampingForces[v] = damping * verts[v].Velocity;
			}
		}

		//更新合力
		for (size_t v = 0; v < nVert; v++)
		{
			if (!verts[v].enabled)
			{
				verts[v].Force = glm::vec3(0);
			}
			else if(v < nVert - 1)
			{
				verts[v].Force += springForces[v] - dampingForces[v] - springForces[v + 1] + dampingForces[v + 1];
			}
			else
			{
				verts[v].Force += springForces[v] - dampingForces[v];
			}
		}

		//更新速度/位置
		for (size_t v = 0; v < nVert; v++)
		{
			if (!verts[v].enabled)
			{
				verts[v].Velocity = glm::vec3(0);
			}
			else
			{
				verts[v].Velocity += verts[v].Force / verts[v].Mass * dt;
				verts[v].Force = glm::vec3(0); //重置
			}
			verts[v].Position += verts[v].Velocity * dt;
			verts[v].OldPosition = verts[v].Position;
		}
	}


}