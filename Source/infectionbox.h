#ifndef INFECTIONBOX_H
#define INFECTIONBOX_H

#include "infection.h"
#include "fancyslider.h"

#include <QObject>
#include <QString>
#include <QGroupBox>
#include <QSlider>

class InfectionBox : public QGroupBox
{
    FancySlider *_carrierContagiosnessSlider;
    FancySlider *_illContagiosnessSlider;
    FancySlider *_deadContagiosnessSlider;
    FancySlider *_carrierIllnessSlider;
    FancySlider *_deadlinessSlider;
    FancySlider *_recoverySlider;
    FancySlider *_initialCarriersSlider;

    static const int MinSliderPos = 0;
    static const int MaxSliderPos = 100;
    static const long MaxInitialCarriers = 1000;

public:
    InfectionBox(const QString &title, QWidget *parent = nullptr);

    Infection::Desc makeInfection() const noexcept;
    void randomize() noexcept;
};

#endif // INFECTIONBOX_H
