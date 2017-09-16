
#include "FTL.h"

FTL::FTL(Hair& h) : ISimulation(h)
{}

FTL::~FTL()
{}

void FTL::update()
{
    constexpr float TimeStep = 1.0f/125.0f; // mentioned as dt
    constexpr float Damping = 0.9f; //sDamping

    const float L0 = Vertex::L0; 
    Strand* restrict str = this->hair.data();
    #pragma omp parallel for schedule(static) firstprivate(str,L0) default(none)
    for(size_t s=0; s < this->hair.size(); s++)
    {
        Vertex* restrict x = str[s].data();
        const size_t nVert = this->hair[s].size();

// these two pragmas were actually meant to vectorize this inner loop
// however, since we have data dependence (we need the predecessing vertex to solve the constraint and the sucessing one for applying the correction vector), we dont see a way to properly vectorize this inner loop.
// @LARS uncomment the pragmas, build with icpc at -O3 to see very strange behaviour
//        #pragma omp simd aligned(x:Alignment)
//        #pragma vector aligned(x)
        for(size_t v=nVert-1; v >= 1; v--) // start with the last vertex, since we its correction vector for its predecessor
        {
            x[v].OldPosition = x[v].Position; // (4)

            // calc new position
            x[v].Position = x[v].OldPosition + TimeStep * x[v].Velocity + x[v].Force * TimeStep * TimeStep; // (1)


            // solve constraints - (2)
            Vertex& pre = str[s][v-1];

            vec3 direction = x[v].Position - pre.Position;
            direction = glm::normalize(direction);

            // backup x's new position before we're going to constraint it
            vec3 pBackup = x[v].Position;

            // constraint the new position of vertex x on a sphere of radius l0 around previous vertex
            x[v].Position = pre.Position + direction * L0;
            // end solve constraint

            // update velocity + final position + force
            x[v].Velocity = ((x[v].Position - x[v].OldPosition) / TimeStep); // (3)

            // correct the velocity with the correction vector computed for previous vertex
            // obviously this is not possible for the last particle, so add 0 in this case
            x[v].Velocity += (v+1>=nVert) ? vec3(0,0,0) : Damping * (-x[v+1].Correction / TimeStep); // (9)


            // correction vector for satisfying the constraint
            // needed when correcting velocity in next iteration
            x[v].Correction = x[v].Position - pBackup; // (6)

            x[v].Force = vec3(0,0,0);
        }
    }
}
