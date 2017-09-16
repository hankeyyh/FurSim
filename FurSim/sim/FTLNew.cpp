#include "FTLNew.h"



FTLNew::FTLNew(Hair &hair, float _sdamping) : ISimulation(hair),
sdamping(_sdamping)
{
}


FTLNew::~FTLNew()
{
}

void FTLNew::update()
{
	float dt = 1.0f / 100.0f;

	// update velocities
	for (size_t s = 0; s < this->hair.size(); s++)
	{
		Vertex* restrict verts = this->hair[s].data();
		const size_t nVert = this->hair[s].size();

		for (size_t v = 0; v < nVert; v++) 
		{
			if (verts[v].enabled == false)
			{
				verts[v].tmp_Position = verts[v].Position;
				continue;
			}
			verts[v].Velocity = verts[v].Velocity + dt * (verts[v].Force / verts[v].Mass);
			verts[v].tmp_Position = verts[v].Position + verts[v].Velocity * dt + verts[v].Force * dt * dt;
			verts[v].Force = vec3(0);
			//verts[v].Velocity *= 0.99;//yyhÔÝÊ±×¢ÊÍµô
		}

		// solve constraints
		vec3 dir;
		vec3 curr_pos;
		for (size_t v = 1; v < nVert; v++)
		{
			Vertex& pa = verts[v - 1];
			Vertex& pb = verts[v];
			curr_pos = pb.tmp_Position;
			dir = pb.tmp_Position - pa.tmp_Position;
			dir = glm::normalize(dir);
			pb.tmp_Position = pa.tmp_Position + dir * Vertex::L0;
			pb.d = curr_pos - pb.tmp_Position; //  - curr_pos
		}

		for (size_t v = 1; v < nVert; v++)
		{
			Vertex& pa = verts[v - 1];
			Vertex& pb = verts[v];

			if (pa.enabled == false)
				continue;

			pa.Velocity = ((pa.tmp_Position - pa.Position) / dt) + sdamping * (pb.d / dt);
			pa.Position = pa.tmp_Position;
		}
		verts[nVert - 1].Velocity = (verts[nVert - 1].tmp_Position - verts[nVert - 1].Position) / dt;
		verts[nVert - 1].Position = verts[nVert - 1].tmp_Position;
	}

	// solve constraints
	/*vec3 dir;
	vec3 curr_pos;
	for (size_t s = 0; s < this->hair.size(); s++)
	{
		Vertex* restrict verts = this->hair[s].data();
		const size_t nVert = this->hair[s].size();

		for (size_t v = 1; v < nVert; v++)
		{
			Vertex& pa = verts[v - 1];
			Vertex& pb = verts[v];
			curr_pos = pb.tmp_Position;
			dir = pb.tmp_Position - pa.tmp_Position;
			dir = glm::normalize(dir);
			pb.tmp_Position = pa.tmp_Position + dir * Vertex::L0;
			pb.d = curr_pos - pb.tmp_Position; //  - curr_pos
		}
	}

	for (size_t s = 0; s < this->hair.size(); s++)
	{
		Vertex* restrict verts = this->hair[s].data();
		const size_t nVert = this->hair[s].size();

		for (size_t v = 1; v < nVert; v++)
		{
			Vertex& pa = verts[v - 1];
			Vertex& pb = verts[v];

			if (pa.enabled == false)
				continue;

			pa.Velocity = ((pa.tmp_Position - pa.Position) / dt) + 0.9f * (pb.d / dt);
			pa.Position = pa.tmp_Position;
		}
		verts[nVert - 1].Velocity = (verts[nVert - 1].tmp_Position - verts[nVert - 1].Position) / dt;
		verts[nVert - 1].Position = verts[nVert - 1].tmp_Position;
	}*/
}

void FTLNew::SetSDamping(float _sDamping)
{
	sdamping = _sDamping;
}