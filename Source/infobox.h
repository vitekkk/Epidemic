#ifndef INFOBOX_H
#define INFOBOX_H

#include "populationgrid.h"

#include <QObject>
#include <QGroupBox>
#include <QString>
#include <QLabel>
#include <array>

class InfoBox : public QGroupBox
{
    std::array<QLabel*, LastGroup> _situationLabels;
    std::array<QString, LastGroup> _labelBaseStrings;
    QLabel*                        _timeLabel;
    QString                        _timeBaseString;
public:
    InfoBox(const QString &title, QWidget *parent = nullptr);
    void setSituationData(const PopulationGrid& g);
};

#endif // INFOBOX_H
