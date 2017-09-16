#pragma once

#include "Vertex.h"

/**
 * Abstract base class for all hair simulation methods to provide a common interface.
 */
class ISimulation
{
protected:
    Hair& hair;

    // this is an interface, even if there were no pure virtual methods, allow
    // construction for child classes only
    ISimulation(Hair& h) : hair(h)
    {}

public:

    // no copy or assignment
    ISimulation(ISimulation const&) = delete;
    ISimulation& operator=(ISimulation const&) = delete;

    // virtual destructor for proper cleanup
    virtual ~ISimulation()
    {}


    /**
     * adds the given force to each and every hair strand
     */
    void addForce(vec3 force);

    /**
     * performs a single time step update of the hair simulation
     */
    virtual void update() = 0;

};
