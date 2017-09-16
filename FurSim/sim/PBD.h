
#include "ISimulation.h"

class PBD : public ISimulation
{
public:
    PBD(Hair& h);
    ~PBD() override;
    void update() override;
};
