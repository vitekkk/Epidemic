#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "epidemicthread.h"
#include "infectionbox.h"
#include "populationbox.h"
#include "infobox.h"
#include "consts.h"

#include <QMainWindow>
#include <QColor>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include <QtGui/QFontDatabase>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtDataVisualization/qutils.h>
#include <QtDataVisualization/q3dbars.h>

using namespace QtDataVisualization;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct BarsSeriesDesc
    {
        QString name;
        QColor color;
    };

    InfectionBox* _infectionBox;
    PopulationBox* _populationBox;
    InfoBox*       _infoBox;
    QSlider* _simSpeedSlider;
    QPushButton *_startButton;
    QPushButton *_stopButton;
    QPushButton *_resetButton;

    Q3DBars     *_bars;
    std::array<QBar3DSeries*, LastGroup> _barsSeries;
    QValue3DAxis *_populationAxis;

    EpidemicThreadPtr _thread;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_resetButton_clicked();

    void run();
    void stop();
    void setData(const PopulationGrid& grid);
    void setSimulationSpeed(int speed);
private:
    void setBarsValueAxis();

    static int gameSpeed2msec(int speed);
    static int msec2GameSpeed(int msec);

};
#endif // MAINWINDOW_H
