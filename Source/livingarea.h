#ifndef LIVINGAREA_H
#define LIVINGAREA_H

#include "consts.h"

struct LivingArea
{
    long healthySusc;
    long healthyUnSusc;
    long healthyRecovered;
    long carrier;
    long ill;
    long dead;
    long currentTime;

    long totalHealthy() const
    {
        return healthySusc + healthyUnSusc + healthyRecovered;
    }

    long totalMobile() const
    {
        return totalHealthy() + carrier;
    }

    long totalAlive() const
    {
        return totalMobile() + ill;
    }

    long total() const
    {
        return totalAlive() + dead;
    }

    long groupTotal(PopulationGroup g) const;
    void zero() noexcept;
};


#endif // LIVINGAREA_H
