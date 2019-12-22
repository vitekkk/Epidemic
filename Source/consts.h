#ifndef CONSTS_H
#define CONSTS_H

#define DEFAULT_SIMULATION_PERIOD 150
#define DEFAULT_SIMILATION_STEP 7200
#define CITIES_AREA_SHARE 20;// 1/CITIES_AREA_SHARE
#define POPULATION_SLIDER_MULTIPLIER 1000
#define DEFAULT_POPULATION_MIGRATION 0.03//30% moves to other locations during a day

#define SIMULATION_SPEED_VS_PERIOD_K	23610.94587
#define SIMULATION_SPEED_VS_PERIOD_B	-195.1219512
#define SIMULATION_SPEED_VS_PERIOD_C	9.75609756

enum PopulationGroup
{
    HealthyGroup = 0
    , CarrierGroup
    , IllGroup
    , DeadGroup
    , TotalGroup
    , LastGroup
};


#endif // CONSTS_H
