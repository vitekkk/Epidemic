#ifndef EPIDEMICTHREAD_H
#define EPIDEMICTHREAD_H

#include "populationgrid.h"
#include "infection.h"

#include <QThread>
#include <boost/shared_ptr.hpp>

class EpidemicThread : public QThread
{
    Q_OBJECT

    Infection _infection;
    PopulationGrid _grid;

public:
    EpidemicThread(const Infection::Desc& infection, const PopulationGrid::Desc& population);
    ~EpidemicThread();

    void setInfection(const Infection::Desc& infec);
    void setPopulation(const PopulationGrid::Desc& pplt);//resets infection
    void setSimulationPeriod(unsigned int p);
    PopulationGrid getGrid() const;

signals:
    void stepDone(const PopulationGrid& g);

protected:
    void run();

public slots:
    void stop();
    void randomize();

private:
    volatile bool               _stopped;
    volatile unsigned int		_msecPeriod;
};

typedef boost::shared_ptr<EpidemicThread> EpidemicThreadPtr;

#endif // EPIDEMICTHREAD_H
