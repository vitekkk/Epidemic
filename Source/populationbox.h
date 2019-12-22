#ifndef POPULATIONBOX_H
#define POPULATIONBOX_H

#include "populationgrid.h"
#include "fancyslider.h"

#include <QObject>
#include <QGroupBox>
#include <QString>

class PopulationBox : public QGroupBox
{
    FancySlider* _populationDensitySlider;
    FancySlider* _susceptiblePartSlider;
    static const int  GridWidth = 12;
    static const int  GridHeight = 12;
    static const int  MaxPopulationDensity = 200;
public:
    PopulationBox(const QString &title, QWidget *parent = nullptr);

    PopulationGrid::Desc makePopulation() const noexcept;
};

#endif // POPULATIONBOX_H
