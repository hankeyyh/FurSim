#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <boost/align/aligned_allocator.hpp>

//yyhÔÝÊ±×¢ÊÍµô
#define restrict /*__restrict__*/

constexpr size_t Alignment = 64;

template<typename T>
using aligned_vector = std::vector<T, boost::alignment::aligned_allocator<T, Alignment> >;

typedef glm::vec3 vec3;

struct Vertex
{
    Vertex(vec3 pos, float m);
//     // whether animating this vertex is enabled or not
//     bool IsEnabled{true};

    // indicates the mass of this particle
    float Mass {1.0f};

    // point in 3D space, where this particle is located
    // just like openGL we use a Right-Handed (Y-up) coordinate system here
    vec3 Position {0.0f,0.0f,0.0f};

    // for PBD we have to memorize the position of this vertex, before it is being changed a following simulation step
    vec3 OldPosition {0.0f,0.0f,0.0f};

	//yyh: for FTL
	vec3 tmp_Position{ 0.0f, 0.0f, 0.0f };
	bool enabled = true;
	vec3 d;

    // the correction vector computed during last run to constraint this->Position on a spehere around its predecessor
    vec3 Correction {0.0f,0.0f,0.0f};

    // holds a force that will be applied during a subsequent simulation step of this particle
    vec3 Force = vec3(0.0f,0.0f,0.0f);

    // we have to remember the velocity applied to this particle during last simulation step
    vec3 Velocity {0.0f,0.0f,0.0f};

    // the l0 length as described in the paper, i.e. the distance to the adjacent vertex
    /*const*/ static float L0; // meant to be const, but it deletes our copy ctor :(
};




typedef aligned_vector<Vertex> Strand;
typedef aligned_vector<Strand> Hair;
