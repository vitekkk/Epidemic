#include "epidemicthread.h"
#include "consts.h"

#include "timer.h"
//#include <iostream>

EpidemicThread::EpidemicThread(const Infection::Desc& infection, const PopulationGrid::Desc& population):
    _infection(infection)
    , _grid(population)
    , _stopped(false)
    , _msecPeriod(DEFAULT_SIMULATION_PERIOD)
{
    _grid.randomize();
    _infection.start(_grid);
}

void EpidemicThread::run()
{
    Timer timer;
    _stopped = false;
    while(!_stopped)
    {
        timer.start();
        _infection.develop(_grid, DEFAULT_SIMILATION_STEP);
        _grid.migrate(DEFAULT_SIMILATION_STEP);
        emit stepDone(_grid);
        unsigned int timeSpent = timer.stop()*1000;
        //std::cout << _msecPeriod << "\t" << timeSpent << std::endl;
        if(timeSpent < _msecPeriod)
            msleep(_msecPeriod - timeSpent);
    }
}

void EpidemicThread::stop()
{
    _stopped = true;
}

void EpidemicThread::setInfection(const Infection::Desc& infec)
{
    _infection = infec;
    _infection.start(_grid);
}

void EpidemicThread::setPopulation(const PopulationGrid::Desc& pplt)
{
    _grid.setPopulation(pplt);
}

void EpidemicThread::randomize()
{
    _grid.randomize();
    //_infection.start(_grid);
}

void EpidemicThread::setSimulationPeriod(unsigned int p)
{
    _msecPeriod = p;
}

EpidemicThread::~EpidemicThread()
{
    QThread::exit();
    QThread::wait();
}

PopulationGrid EpidemicThread::getGrid() const
{
    return _grid;
}
