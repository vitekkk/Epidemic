#include "infection.h"
#include "livingarea.h"
#include "randomtoolkit.h"

#include <algorithm>
#include <cassert>

static const float ContaigiousnessCoef = 0.0001;
static const float DeadlinessCoef = 0.5;
static const float IllnessCoef = 0.5;
static const float RecoveryCoef = 0.5;

static long randomRound(double d) noexcept
{
    static std::uniform_real_distribution<> distr(0., 1.);
    long res = (long)d;
    if(distr(RandomToolkit::getMersienneGenerator()) < (d - res))
        ++res;
    return res;
}

Infection::Infection(const Infection::Desc& d):
    _desc(d)
{

}

void Infection::reset(const Infection::Desc& d)
{
    _desc = d;
}

void Infection::start(PopulationGrid& g, int locationsCount)
{
    static std::uniform_int_distribution<> distrX(0, g.height() - 1);
    static std::uniform_int_distribution<> distrY(0, g.width() - 1);
    std::mt19937& gen = RandomToolkit::getMersienneGenerator();
    for(int i = 0; i < locationsCount; ++i)
    {
        LivingArea& a = g._grid[distrX(gen)][distrY(gen)];
        if(a.healthySusc > 0)
        {
            long initialCarriers = std::min(_desc.initialCarriers, a.healthySusc);
            a.carrier += initialCarriers;
            a.healthySusc -= initialCarriers;
        }
    }
}


LivingArea Infection::develop(const LivingArea& area, int seconds) const noexcept
{
    static double day = 86400.;
    LivingArea result(area);
    result.currentTime += seconds;
    double dt = seconds / day;
    double recovered = std::min(RecoveryCoef*area.ill * _desc.recoveryRate * dt, (double)area.ill);
    double dead = std::min(DeadlinessCoef*area.ill * _desc.deathRate * dt, (double)area.ill);
    if(dead > recovered)
    {
        long d = randomRound(dead);
        result.dead += d;
        result.healthyRecovered += std::min((long)recovered, area.ill - d);
    }
    else
    {
        long r = randomRound(recovered);
        result.healthyRecovered += r;
        result.dead += std::min((long)dead, area.ill - r);
    }
    result.ill -= (result.dead - area.dead) + (result.healthyRecovered - area.healthyRecovered);

    long newIll = randomRound(std::min(IllnessCoef*dt * _desc.carrier2illRate, 1.) * area.carrier);
    result.ill += newIll;
    result.carrier -= newIll;

    double infectionRate = ContaigiousnessCoef*dt*(area.carrier*_desc.carrierInfectRate +
                               area.ill*_desc.illInfectRate +
                               area.dead*_desc.deadInfectRate);
    infectionRate = std::min(infectionRate, 1.);
    long newCarriers = randomRound(infectionRate * area.healthySusc);
    result.healthySusc -= newCarriers;
    result.carrier += newCarriers;

    assert(result.total() == area.total());
    assert(result.healthySusc >= 0);
    assert(result.carrier >= 0);
    assert(result.ill >= 0);

    return result;
}

void Infection::develop(PopulationGrid& g, int seconds)
{
    for(auto& i: g._grid)
        for(auto& j: i)
            j = develop(j, seconds);
}

Infection& Infection::operator=(const Infection::Desc& d)
{
    _desc = d;
    return *this;
}
