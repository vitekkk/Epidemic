#include "infobox.h"
#include <QVBoxLayout>

InfoBox::InfoBox(const QString &title, QWidget *parent):
    QGroupBox(title, parent)
  , _labelBaseStrings({"Healthy:\t", "Carriers:\t", "Ill:\t", "Dead:\t", "Total:\t"})
  , _timeBaseString("Time:\t")
{
    QVBoxLayout *vBox = new QVBoxLayout;

    for(size_t i = 0; i < LastGroup; ++i)
    {
        _situationLabels[i] = new QLabel(_labelBaseStrings[i], this);
        vBox->addWidget(_situationLabels[i]);
    }
    _timeLabel= new QLabel(_timeBaseString, this);
    vBox->addWidget(_timeLabel);
    setLayout(vBox);
}

void InfoBox::setSituationData(const PopulationGrid& g)
{
    PopulationGrid::SituationDesc d = g.situation();
    long totalPeople = 0;
    for(size_t i = 0; i < LastGroup; ++i)
    {
        QString text(_labelBaseStrings[i]);
        text += "%1";
        _situationLabels[i]->setText(text.arg(d.peopleNumber[i]));
        totalPeople += d.peopleNumber[i];
    }
    _situationLabels[TotalGroup]->setText((QString(_labelBaseStrings[TotalGroup]) + "%1").arg(totalPeople));
    _timeLabel->setText((QString(_timeBaseString) + "%1 h").arg(g.getTime() / 3600));
}
