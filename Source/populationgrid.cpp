#include "populationgrid.h"
#include "consts.h"

#include "randomtoolkit.h"
#include <algorithm>
#include <cassert>

PopulationGrid::Desc::Desc():
    nonSuceptiblePart(0.)
  , maxPopulationDensity(1 * POPULATION_SLIDER_MULTIPLIER)
  , width(10)
  , height(10)
{
}

PopulationGrid::PopulationGrid():
    _desc()
  ,_grid(_desc.width, std::vector<LivingArea>(_desc.height))
{
    zero();
    discoverRoutes();
}

PopulationGrid::PopulationGrid(const Desc& d):
    _grid(d.width, std::vector<LivingArea>(d.height))
    , _desc(d)
{
    randomize();
    discoverRoutes();
}

void PopulationGrid::discoverRoutes()
{
    _allLocations.clear();
    _allRoutes.clear();
    for(size_t i = 0; i < width(); ++i)
        for(size_t j = 0; j < height(); ++j)
            _allLocations.push_back(std::make_pair(i, j));
    for(size_t i = 0; i < _allLocations.size(); ++i)
        for(size_t j = i + 1; j < _allLocations.size(); ++j)
            _allRoutes.push_back(std::make_pair(_allLocations[i], _allLocations[j]));
}

void PopulationGrid::zero()
{
    for(auto& i : _grid)
        for(auto& j : i)
            j.zero();
}

const LivingArea& PopulationGrid::at(int i, int j) const noexcept
{
    return _grid[i][j];
}

void PopulationGrid::colonizeLocation(const Location& l, long population)
{
    _grid[l.first][l.second].healthyUnSusc = population * _desc.nonSuceptiblePart;
    _grid[l.first][l.second].healthySusc = population - _grid[l.first][l.second].healthyUnSusc;
}

void PopulationGrid::colonizeRoam(const Location& l, long maxPopulation)
{
    static std::uniform_real_distribution<> distr(0., 1.);

    std::vector<Location> newLocations;
    newLocations.push_back(std::make_pair(l.first + 1, l.second));
    newLocations.push_back(std::make_pair(l.first - 1, l.second));
    newLocations.push_back(std::make_pair(l.first, l.second + 1));
    newLocations.push_back(std::make_pair(l.first, l.second - 1));
    for(const auto& location: newLocations)
    {
        if(location.first >= 0 && location.second >= 0 && location.first < width() && location.second < height())
        {
            long colonists = distr(RandomToolkit::getMersienneGenerator()) * (double)maxPopulation;
            if(colonists > _grid[location.first][location.second].total())
            {
                colonizeLocation(location, colonists);
                colonizeRoam(location, colonists);
            }
        }
    }
}

void PopulationGrid::randomize()
{
    static std::uniform_int_distribution<> distrX(0, width() - 1);
    static std::uniform_int_distribution<> distrY(0, height() - 1);
    std::mt19937& gen = RandomToolkit::getMersienneGenerator();

    zero();
    int area = height()*width();
    int citiesCount = area/CITIES_AREA_SHARE;
    if(!citiesCount)
        citiesCount = 1;
    std::vector<Location> citiesLocations;
    for(size_t i = 0; i < citiesCount; ++i)
        citiesLocations.push_back(std::make_pair(distrX(gen), distrY(gen)));
    for(const auto& location: citiesLocations)
    {
        colonizeLocation(location, _desc.maxPopulationDensity);
        colonizeRoam(location, _desc.maxPopulationDensity);
    }
}

void PopulationGrid::setPopulation(const Desc& d)
{
    _desc = d;
    for(auto& l: _allLocations)
    {
        LivingArea& area = _grid[l.first][l.second];
        long total = area.total();
        long nonSusceptible = total * d.nonSuceptiblePart;
        area = {total - nonSusceptible, nonSusceptible, 0, 0, 0, 0, 0};
    }
}

void PopulationGrid::migrate(int seconds)
{
    std::shuffle(_allRoutes.begin(), _allRoutes.end(), RandomToolkit::getMersienneGenerator());
    for(const MigrationRoute& r: _allRoutes)
    {
        LivingArea* a = &(_grid[r.first.first][r.first.second]);
        LivingArea* b = &(_grid[r.second.first][r.second.second]);
        float distance = sqrt((r.first.first - r.second.first)*(r.first.first - r.second.first)
                              +(r.first.second - r.second.second)*(r.first.second - r.second.second));
        long maxTravellers = std::min(a->totalMobile(), b->totalMobile())/distance;
        assert(maxTravellers >= 0);
        double migrationIntensity =((double)a->carrier)/a->total() - ((double)b->carrier)/b->total();
        if(migrationIntensity < 0.)
        {
            std::swap(a, b);
            migrationIntensity *= -1.;
        }
        long travellers = migrationIntensity * std::min(a->carrier, b->healthySusc + b->healthyUnSusc) *
                DEFAULT_POPULATION_MIGRATION;
        long nonsusceptibleTravellers = travellers * ((double)b->healthyUnSusc/(b->healthySusc + b->healthyUnSusc));
        long susceptibleTravellers = travellers - nonsusceptibleTravellers;
        a->carrier -= travellers;
        b->carrier += travellers;
        a->healthySusc += susceptibleTravellers;
        a->healthyUnSusc += nonsusceptibleTravellers;
        b->healthySusc -= susceptibleTravellers;
        b->healthyUnSusc -= nonsusceptibleTravellers;
        assert(travellers >= 0);
        assert(nonsusceptibleTravellers >= 0 && nonsusceptibleTravellers <= travellers);
        assert(a->carrier >= 0);
        assert(b->healthySusc >= 0);
        assert(b->healthyUnSusc >= 0);
    }
}

PopulationGrid::SituationDesc PopulationGrid::situation() const noexcept
{
    SituationDesc r = {0, 0, 0, 0};
    for(const auto& l: _allLocations)
    {
        r.peopleNumber[HealthyGroup] += _grid[l.first][l.second].totalHealthy();
        r.peopleNumber[CarrierGroup] += _grid[l.first][l.second].carrier;
        r.peopleNumber[IllGroup] += _grid[l.first][l.second].ill;
        r.peopleNumber[DeadGroup] += _grid[l.first][l.second].dead;
    }
    return r;
}

