#include "infectionbox.h"
#include "randomtoolkit.h"

#include <QLabel>
#include <QVBoxLayout>
#include <vector>

InfectionBox::InfectionBox(const QString &title, QWidget *parent):
    QGroupBox(title, parent)
{
    std::vector<QSlider*> sliders;
    _carrierContagiosnessSlider = new FancySlider(Qt::Horizontal, this);
    _illContagiosnessSlider = new FancySlider(Qt::Horizontal, this);
    _deadContagiosnessSlider = new FancySlider(Qt::Horizontal, this);
    _carrierIllnessSlider = new FancySlider(Qt::Horizontal, this);
    _deadlinessSlider = new FancySlider(Qt::Horizontal, this);
    _recoverySlider = new FancySlider(Qt::Horizontal, this);
    _initialCarriersSlider = new FancySlider(Qt::Horizontal, this);
    sliders.push_back(_carrierContagiosnessSlider);
    sliders.push_back(_illContagiosnessSlider);
    sliders.push_back(_deadContagiosnessSlider);
    sliders.push_back(_carrierIllnessSlider);
    sliders.push_back(_deadlinessSlider);
    sliders.push_back(_recoverySlider);
    for(auto& slider: sliders)
    {
        slider->setTickInterval(20);
        slider->setTickPosition(QSlider::TicksBelow);
        slider->setMinimum(MinSliderPos);
        slider->setValue(0);
        slider->setMaximum(MaxSliderPos);
    }
    _initialCarriersSlider->setMinimum(1);
    _initialCarriersSlider->setMaximum(MaxInitialCarriers);
    _initialCarriersSlider->setValue(MaxInitialCarriers);
    _initialCarriersSlider->setTickPosition(QSlider::TicksBelow);
    _initialCarriersSlider->setTickInterval(MaxInitialCarriers / 5);
    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(new QLabel(QStringLiteral("Carrier contaigiousness")));
    vBox->addWidget(_carrierContagiosnessSlider, 0, Qt::AlignTop);
    vBox->addWidget(new QLabel(QStringLiteral("Ill contaigiousness")));
    vBox->addWidget(_illContagiosnessSlider, 0, Qt::AlignTop);
    vBox->addWidget(new QLabel(QStringLiteral("Dead contaigiousness")));
    vBox->addWidget(_deadContagiosnessSlider, 0, Qt::AlignTop);
    vBox->addWidget(new QLabel(QStringLiteral("Carrier illness rate")));
    vBox->addWidget(_carrierIllnessSlider, 0, Qt::AlignTop);
    vBox->addWidget(new QLabel(QStringLiteral("Death rate")));
    vBox->addWidget(_deadlinessSlider, 0, Qt::AlignTop);
    vBox->addWidget(new QLabel(QStringLiteral("Recovery rate")));
    vBox->addWidget(_recoverySlider, 0, Qt::AlignTop);
    vBox->addWidget(new QLabel(QStringLiteral("Initial Carriers")));
    vBox->addWidget(_initialCarriersSlider, 0, Qt::AlignTop);
    vBox->addStretch(1);
    setLayout(vBox);
}

Infection::Desc InfectionBox::makeInfection() const noexcept
{
    double sliderSpan = MaxSliderPos - MinSliderPos;
    Infection::Desc d = {
        _carrierContagiosnessSlider->value() / sliderSpan
        , _illContagiosnessSlider->value() / sliderSpan
        , _deadContagiosnessSlider->value() / sliderSpan
        , _carrierIllnessSlider->value() / sliderSpan
        , _deadlinessSlider->value() / sliderSpan
        , _recoverySlider->value() / sliderSpan
        , _initialCarriersSlider->value()
    };
    return d;
}

void InfectionBox::randomize() noexcept
{
    std::mt19937& gen = RandomToolkit::getMersienneGenerator();
    std::uniform_int_distribution<> randomPercent(MinSliderPos, MaxSliderPos);
    _carrierContagiosnessSlider->setValue(randomPercent(gen));
    _illContagiosnessSlider->setValue(randomPercent(gen));
    _deadContagiosnessSlider->setValue(randomPercent(gen));
    _carrierIllnessSlider->setValue(randomPercent(gen));
    _deadlinessSlider->setValue(randomPercent(gen));
    _recoverySlider->setValue(randomPercent(gen));
    _initialCarriersSlider->setValue(randomPercent(gen)*(MaxInitialCarriers / 100));
}
