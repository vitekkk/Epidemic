#ifndef INFECTION_H
#define INFECTION_H

#include "populationgrid.h"

class Infection
{
public:

    struct Desc
    {
        double carrierInfectRate;
        double illInfectRate;
        double deadInfectRate;
        double carrier2illRate;
        double deathRate;
        double recoveryRate;
        long initialCarriers;
    };
private:
    Desc _desc;
public:
    Infection(const Desc& d);

    LivingArea develop(const LivingArea& area, int seconds) const noexcept;

    void start(PopulationGrid& g, int locationsCount = 1);
    void develop(PopulationGrid& g, int seconds);

    void reset(const Desc& d);

    Infection& operator=(const Desc& d);
};

#endif // INFECTION_H
