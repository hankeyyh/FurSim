#pragma once
#include "Vertex.h"

//pearl system
class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	bool IsColliding(Vertex& ballA, Vertex& ballB, float radiusA, float radiusB);
	void HandleCollision(Vertex& ballA, Vertex& ballB, float radiusA, float radiusB, float ks);
	void Update(Hair& hair, Vertex &object, float objectRadius, float ks);
};

