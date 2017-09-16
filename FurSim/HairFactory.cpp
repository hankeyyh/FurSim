#include "HairFactory.h"
#include <time.h>


using namespace std;

Hair HairFactory::GrowHairOnCircle(const size_t nStrands, const size_t nVert, float interDis)
{
    Hair hair;//(nStrands, vector<Vertex>(nVert, Vertex() ))
    vec3 startPos(0,0.5,0);
    //const double Len = 0.5;
    Vertex::L0 = interDis;//particle之间距离

	float radius = 6.0f;

    for(size_t s=0; s<nStrands; s++)
    {
        hair.push_back(aligned_vector<Vertex>());
        Strand& strand = hair.back();

		double sigma = (360.0) / nStrands * M_PI / 180.0;

		Vertex particle(startPos, 1.0f);
		particle.Position.x = startPos.x + radius * cos(sigma * s);
		particle.Position.z = startPos.z + radius * sin(sigma * s);
        for(size_t v=0; v<nVert; v++)
        {
            strand.push_back(particle);

            // place next particle above the current one
            particle.Position.y = startPos.y + Vertex::L0 * v;
        }

        // go to the right
        //startPos.x += 0.007; // some rand val here
		strand[0].enabled = false;
    }
	
    return hair;
}

Hair HairFactory::GrowHairOnSphere(const size_t nStrands, const size_t nVert, float interDis)
{
	Hair hair;
	vec3 startPos(-0.87, 0, 0);
	//const double Len = 0.5;
	Vertex::L0 = interDis;//particle之间距离

	float radius = 3.0f;

	for (size_t s = 0; s < nStrands; s++)
	{
		hair.push_back(aligned_vector<Vertex>());
		Strand& strand = hair.back();

		Vertex particle(startPos, 1.0f);

		int N = 999;//随机数为三位小数
		float u = rand() % (N + 1) / (float)(N + 1);
		float v = rand() % (N + 1) / (float)(N + 1);
		float theta = 2 * M_PI * u;
		float phi = acos(2 * v - 1);
		particle.Position.x = startPos.x + radius * sin(theta) * sin(phi);
		particle.Position.z = startPos.z + radius * cos(theta) * sin(phi);
		particle.Position.y = startPos.y + radius * cos(phi);


		for (size_t v = 0; v<nVert; v++)
		{
			strand.push_back(particle);

			// place next particle above the current one
			particle.Position.y = startPos.y + radius * cos(phi) + Vertex::L0 * v;
		}
		strand[0].enabled = false;
	}

	return hair;
}

Hair HairFactory::GrowHairOnPlane(const size_t nStrands, const size_t nVert, float interDis)
{
    Hair hair;//(nStrands, vector<Vertex>(nVert, Vertex() ))
    vec3 startPos(-6.0, 2, 0);
    const size_t Layers = 1; //几个平面
    //const double Len = 0.5;

    //Vertex::L0 = Len/nVert;
	Vertex::L0 = interDis;


    for(size_t layer=0; layer < Layers; layer++)
    {
        //startPos.x = -0.8 +  (layer*0.015 / Layers);
        //startPos.z = layer*0.5;
        for(size_t s=0; s<nStrands; s++)
        {
            hair.push_back(aligned_vector<Vertex>());
            Strand& strand = hair.back();

            Vertex particle(startPos, 1.0f);
            for(size_t v=0; v<nVert; v++)
            {
                strand.push_back(particle);

                // place next particle above the current one
                particle.Position.y = startPos.y - Vertex::L0 * v;
            }

            // go to the right
            startPos.x += 0.03; // some rand val here
			strand[0].enabled = false;
        }
    }

    return hair;
}


Hair HairFactory::GrowHairOnRow(const size_t nStrands, const size_t nVert, float interDis)
{
	Hair hair;
	Vertex::L0 = interDis;
	vec3 startPos(-6.0, nVert * interDis * 0.5, 0);
	float mass = rand() % 100 / (float)10.0f;

	for (size_t s = 0; s < nStrands; s++)
	{
		hair.push_back(aligned_vector<Vertex>());
		Strand& strand = hair.back();
	
		Vertex particle(startPos, /*mass*/0.05);
		for (size_t v = 0; v < nVert; v++)
		{
			strand.push_back(particle);
			particle.Position.y -= Vertex::L0;
			particle.Mass = (rand() % 4 + 2) / (float)100.0f;
		}
		startPos.x += 0.03;
		strand[0].enabled = false;
	}

	return hair;
}

Hair HairFactory::GrowHairOnDefinedPos(const size_t nVert, float interDis, std::vector<glm::vec3>& position)
{
	Hair hair;
	Vertex::L0 = interDis;
	


	for (size_t w = 0; w < position.size(); w++)
	{
		hair.push_back(aligned_vector<Vertex>());
		Strand& strand = hair.back();
		//float mass = (rand() % 4 + 2) / (float)100.0f; //0.02~0.05随机数
		Vertex particle(position[w], /*mass*/0.05);
		for (size_t v = 0; v < nVert; v++)
		{
			
			strand.push_back(particle);
			particle.Position.y -= Vertex::L0;
			particle.Mass = (rand() % 4 + 2) / (float)100.0f;
		}
		strand[0].enabled = false;
	}

	return hair;
}