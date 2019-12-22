#ifndef EPIDEMICGRID_H
#define EPIDEMICGRID_H

#include "livingarea.h"

#include <vector>

class PopulationGrid
{
    friend class Infection;
public:
    struct Desc
    {
        double nonSuceptiblePart; /*[0., 1.]*/
        long maxPopulationDensity;
        int width;
        int height;
        Desc();
    };

    struct SituationDesc
    {
        long peopleNumber[LastGroup];
    };

private:
    typedef std::pair<int, int> Location;
    typedef std::pair<Location, Location> MigrationRoute;

    std::vector<Location> _allLocations;
    std::vector<MigrationRoute> _allRoutes;

    void discoverRoutes();
    Desc _desc;

    std::vector<std::vector<LivingArea> > _grid;

    void zero();
    void colonizeLocation(const Location& l, long population);
    void colonizeRoam(const Location& l, long maxPopulation);

public:
    int width() const noexcept
    {
        return _grid.size();
    }

    int height() const noexcept
    {
        if(_grid.empty())
            return 0;
        return _grid[0].size();
    }

    long maxPopulation() const noexcept
    {
        return _desc.maxPopulationDensity;
    }

    void setPopulation(const Desc& d);
    void randomize();
    void migrate(int seconds);

    PopulationGrid();
    PopulationGrid(const Desc& desc);

    const LivingArea& at(int i, int j) const noexcept;

    long getTime() const noexcept {return at(0, 0).currentTime; }

    SituationDesc situation() const noexcept;
};

#endif // EPIDEMICGRID_H
