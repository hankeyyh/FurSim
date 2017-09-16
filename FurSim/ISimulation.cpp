
#include "ISimulation.h"

void ISimulation::addForce(vec3 f)
{
    Strand* restrict str = this->hair.data();

    #pragma omp parallel for schedule(static)
    for(size_t s=0; s < this->hair.size(); s++)
    {
        Vertex* restrict vert = str[s].data();
        #pragma omp simd aligned(vert:Alignment)
        #pragma vector aligned
        for(size_t v=0; v < str[s].size(); v++)
        {
			if(vert[v].enabled)
				vert[v].Force += f;
        }
    }
}