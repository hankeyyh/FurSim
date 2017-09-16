
#include "ISimulation.h"

class FTL : public ISimulation
{
public:
    FTL(Hair& h);
    ~FTL() override;
    
    void update() override;
};