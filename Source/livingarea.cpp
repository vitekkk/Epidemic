#include "livingarea.h"

void LivingArea::zero() noexcept
{
    healthySusc = 0;
    healthyUnSusc = 0;
    healthyRecovered = 0;
    carrier = 0;
    ill = 0;
    dead = 0;
    currentTime= 0;
}

long LivingArea::groupTotal(PopulationGroup g) const
{
    switch (g)
    {
    case HealthyGroup:
        return totalHealthy();
    case CarrierGroup:
        return carrier;
    case IllGroup:
        return ill;
    case DeadGroup:
        return dead;
    case TotalGroup:
        return total();
    default:
        return 0;
    }
}
