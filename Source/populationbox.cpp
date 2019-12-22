#include "populationbox.h"
#include "consts.h"

#include <QLabel>
#include <QVBoxLayout>

PopulationBox::PopulationBox(const QString &title, QWidget *parent):
    QGroupBox(title, parent)
{
    _populationDensitySlider = new FancySlider(Qt::Horizontal, this);
    _populationDensitySlider->setTickInterval(MaxPopulationDensity / 5);
    _populationDensitySlider->setTickPosition(QSlider::TicksBelow);
    _populationDensitySlider->setMinimum(1);
    _populationDensitySlider->setValue(MaxPopulationDensity / 2);
    _populationDensitySlider->setMaximum(MaxPopulationDensity);

    _susceptiblePartSlider = new FancySlider(Qt::Horizontal, this);
    _susceptiblePartSlider->setTickInterval(20);
    _susceptiblePartSlider->setTickPosition(QSlider::TicksBelow);
    _susceptiblePartSlider->setMinimum(0);
    _susceptiblePartSlider->setValue(80);
    _susceptiblePartSlider->setMaximum(100);

    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(new QLabel(QString("").sprintf("Maximum population density, x%d",  POPULATION_SLIDER_MULTIPLIER)));
    vBox->addWidget(_populationDensitySlider, 0, Qt::AlignTop);
    vBox->addWidget(new QLabel(QStringLiteral("Susceptible population part, %")));
    vBox->addWidget(_susceptiblePartSlider, 0, Qt::AlignTop);
    vBox->addStretch(1);
    setLayout(vBox);
}

PopulationGrid::Desc PopulationBox::makePopulation() const noexcept
{
    PopulationGrid::Desc d;
    d.nonSuceptiblePart = 1. - (_susceptiblePartSlider->value() / 100.);
    d.maxPopulationDensity = _populationDensitySlider->value() * POPULATION_SLIDER_MULTIPLIER;
    d.width = GridWidth;
    d.height = GridHeight;
    return d;
}
