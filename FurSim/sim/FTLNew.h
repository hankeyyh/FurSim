#pragma once
#include "ISimulation.h"


class FTLNew : public ISimulation
{
public:
	FTLNew(Hair &hair, float _sdamping);
	virtual ~FTLNew();
	void update() override;
	void SetSDamping(float _sDamping);

public:
	float sdamping;
};

