#pragma once
#include "ISimulation.h"

class MassSpring : public ISimulation
{
public:
	MassSpring(Hair& h, float _springConstant, float _length);
	~MassSpring() override;
	void update() override;

	void ResetForce(); //reset force = 0
	void Solve(); //apply force
	void Simulate(float dt);//pos&vel change with time

public:
	float length;
	float airFriction;
	float springConstant;
	float inertia;
	float maxLength;
};

