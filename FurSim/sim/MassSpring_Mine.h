#pragma once
#include "ISimulation.h"
class MassSpring_Mine : public ISimulation
{
public:
	MassSpring_Mine(Hair& h, float _stiffness, float _damping);
	~MassSpring_Mine() override;
	void update() override;

public:
	float stiffness;
	float damping;
};

