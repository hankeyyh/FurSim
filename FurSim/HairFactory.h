#pragma once

#include "Vertex.h"

class HairFactory
{
public:
    // no object
    HairFactory() = delete;
    // no copy
    HairFactory(const HairFactory&) = delete;
    // no assign
    HairFactory& operator=(const HairFactory&) = delete;


    static Hair GrowHairOnCircle(const size_t nStrands, const size_t nVert, float interDis);
	static Hair GrowHairOnSphere(const size_t nStrands, const size_t nVert, float interDis);
    static Hair GrowHairOnPlane(const size_t nStrands, const size_t nVert, float interDis);

	static Hair GrowHairOnRow(const size_t nStrands, const size_t nVert, float interDis);
	static Hair GrowHairOnDefinedPos(const size_t nVert, float interDis, std::vector<glm::vec3>& position);

// @idea another growhair method may receive a kind of 2d black/white texture that specifies where to grow strands
};
