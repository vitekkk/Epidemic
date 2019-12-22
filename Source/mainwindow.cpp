#include "mainwindow.h"
#include "infectionbox.h"
#include "populationbox.h"
#include "consts.h"

#include <array>
#include <QString>
#include <QGroupBox>
#include <QMetaType>
#include <QtGui/QScreen>
#include <QtWidgets/QHBoxLayout>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtDataVisualization/qutils.h>
#include <QtDataVisualization/q3dbars.h>

using namespace QtDataVisualization;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _barsSeries({new QBar3DSeries, new QBar3DSeries, new QBar3DSeries, new QBar3DSeries, new QBar3DSeries})
    , _populationAxis(new QValue3DAxis)
{
    //QSurfaceFormat format;

    _bars = new Q3DBars(/*&format*/);
    if (!_bars->hasContext())
        throw std::string("Couldn't initialize the OpenGL context.");
    QWidget *container = QWidget::createWindowContainer(_bars);
    QSize screenSize = _bars->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);
    _bars->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftMedium);
    _bars->activeTheme()->setBackgroundEnabled(false);
    _bars->activeTheme()->setFont(QFont("Times New Roman", 10));
    _bars->activeTheme()->setLabelBackgroundEnabled(true);
    _bars->setMultiSeriesUniform(true);


    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    setCentralWidget(widget);

    _infectionBox = new InfectionBox(QStringLiteral("Infection"), widget);
    _infectionBox->randomize();
    _populationBox = new PopulationBox(QStringLiteral("Population"), widget);

    QGroupBox* simulationBox = new QGroupBox("Simulation", widget);
    QVBoxLayout* simLayout = new QVBoxLayout();
    _simSpeedSlider = new QSlider(Qt::Horizontal, simulationBox);
    _startButton = new QPushButton(simulationBox);
    _startButton->setText(QStringLiteral("Start simulation"));
    _stopButton = new QPushButton(simulationBox);
    _stopButton->setText(QStringLiteral("Stop simulation"));
    _resetButton = new QPushButton(simulationBox);
    _resetButton->setText(QStringLiteral("Reset simulation"));

    simLayout->addWidget(new QLabel(QStringLiteral("Speed")));
    simLayout->addWidget(_simSpeedSlider, 0, Qt::AlignTop);
    simLayout->addWidget(_startButton, 0, Qt::AlignTop);
    simLayout->addWidget(_stopButton, 0, Qt::AlignTop);
    simLayout->addWidget(_resetButton, 0, Qt::AlignTop);
    simulationBox->setLayout(simLayout);

    _infoBox = new InfoBox("Info", this);

    vLayout->addWidget(_infectionBox, 0, Qt::AlignTop);
    vLayout->addWidget(_populationBox, 0, Qt::AlignTop);
    vLayout->addWidget(_infoBox, 0, Qt::AlignTop);
    vLayout->addWidget(simulationBox, 0, Qt::AlignTop);
    vLayout->addStretch(1);

    Q3DTheme *currentTheme = _bars->activeTheme();
    currentTheme->setType(Q3DTheme::ThemeIsabelle);

    BarsSeriesDesc seriesNames[] =
    {
        {"Healthy", QColor(0, 200, 0)}
        , {"Carriers", QColor(200, 200, 0)}
        , {"Ill", QColor(200, 0, 0)}
        , {"Deceased", QColor(70, 0, 0)}
        , {"Total", QColor(150, 150, 150)}
    };
    for(size_t i =0; i < LastGroup; ++i)
    {
        _barsSeries[i]->setItemLabelFormat(seriesNames[i].name);
        _barsSeries[i]->setBaseColor(seriesNames[i].color);
        _barsSeries[i]->setMesh(QAbstract3DSeries::MeshBevelBar);
        _barsSeries[i]->setMeshSmooth(false);
        _barsSeries[i]->setVisible(true);
        _bars->addSeries(_barsSeries[i]);
    }

    QObject::connect(_startButton, &QPushButton::clicked, this,
                     &MainWindow::on_startButton_clicked);
    QObject::connect(_stopButton, &QPushButton::clicked, this,
                     &MainWindow::on_stopButton_clicked);
    QObject::connect(_resetButton, &QPushButton::clicked, this,
                     &MainWindow::on_resetButton_clicked);

    _startButton->setEnabled(true);
    _resetButton->setEnabled(true);
    _stopButton->setEnabled(false);
    _simSpeedSlider->setValue(MainWindow::msec2GameSpeed(DEFAULT_SIMULATION_PERIOD));

    _thread = EpidemicThreadPtr(new EpidemicThread(_infectionBox->makeInfection(),
                                                   _populationBox->makePopulation()));
    connect(_thread.get(), SIGNAL(stepDone(const PopulationGrid&)), SLOT(setData(const PopulationGrid&)));
    connect(_simSpeedSlider, SIGNAL(valueChanged(int)), SLOT(setSimulationSpeed(int)));
    setData(_thread->getGrid());
    setBarsValueAxis();
    qRegisterMetaType<PopulationGrid>("PopulationGrid");
    show();
}

MainWindow::~MainWindow()
{
    delete _bars;
}

void MainWindow::on_startButton_clicked()
{
    if(_populationBox->isEnabled())
        _thread->setPopulation(_populationBox->makePopulation());
    if(_infectionBox->isEnabled())
        _thread->setInfection(_infectionBox->makeInfection());
    _populationBox->setDisabled(true);
    _infectionBox->setDisabled(true);
    _resetButton->setDisabled(true);
    _startButton->setDisabled(true);
    _stopButton->setEnabled(true);
    run();
}

void MainWindow::on_stopButton_clicked()
{
    stop();
    _startButton->setEnabled(true);
    _stopButton->setEnabled(false);
    _resetButton->setEnabled(true);
    _populationBox->setDisabled(true);
    _infectionBox->setDisabled(true);
}

void MainWindow::on_resetButton_clicked()
{
    _startButton->setEnabled(true);
    _stopButton->setEnabled(false);
    _populationBox->setDisabled(false);
    _infectionBox->setDisabled(false);
    _thread->randomize();
    _thread->setPopulation(_populationBox->makePopulation());
    _thread->setInfection(_infectionBox->makeInfection());
    setData(_thread->getGrid());
    setBarsValueAxis();
}

void MainWindow::setBarsValueAxis()
{
    long maxValue = (long)(_thread->getGrid().maxPopulation()) / 1000 + 1;
    _populationAxis->setTitle("Individuals x1000");
    _populationAxis->setSegmentCount(10);
    _populationAxis->setSubSegmentCount(2);
    _populationAxis->setRange(0, maxValue);
    _populationAxis->setLabelFormat(QString(QStringLiteral("%d ")));
    _populationAxis->setLabelAutoRotation(30.0f);
    _populationAxis->setTitleVisible(true);
    _bars->setValueAxis(_populationAxis);
}

void MainWindow::run()
{
    _thread->start();
}

void MainWindow::stop()
{
    _thread->stop();
    _thread->wait();
}

void MainWindow::setData(const PopulationGrid& g)
{
    QStringList xLabels;
    QStringList yLabels;
    QString baseSting("%1");
    for(size_t i = 0; i < g.width(); ++i)
        xLabels.append(baseSting.arg(i));
    for(size_t j = 0; j < g.height(); ++j)
        yLabels.append(baseSting.arg((char)('A' + j)));
    QBarDataArray *dataSet[LastGroup];
    std::array<QBarDataRow*, LastGroup> dataRow;
    for(int serie = 0; serie < (int)LastGroup; ++serie)
    {
        dataSet[serie] = new QBarDataArray;
        dataSet[serie]->reserve(g.width());
    }
    for(size_t i = 0; i < g.width(); ++i)
    {
        for(int serie = 0; serie < (int)LastGroup; ++serie)
        {
            dataRow[serie] = new QBarDataRow(g.height());
            for(size_t j = 0; j < g.height(); ++j)
                (*dataRow[serie])[j].setValue(g.at(i, j).groupTotal((PopulationGroup)serie)/POPULATION_SLIDER_MULTIPLIER);
        }
        for(int serie = 0; serie < (int)LastGroup; ++serie)
            (dataSet[serie])->append(dataRow[serie]);
    }
    for(int serie = 0; serie < (int)LastGroup; ++serie)
        _barsSeries[serie]->dataProxy()->resetArray(dataSet[serie], xLabels, yLabels);
    _infoBox->setSituationData(g);
}

int MainWindow::gameSpeed2msec(int speed)
{
    return (int)(SIMULATION_SPEED_VS_PERIOD_K/(speed + SIMULATION_SPEED_VS_PERIOD_C) + SIMULATION_SPEED_VS_PERIOD_B + 0.5);
}

int MainWindow::msec2GameSpeed(int msec)
{
    return (int)(SIMULATION_SPEED_VS_PERIOD_K/(msec - SIMULATION_SPEED_VS_PERIOD_B) - SIMULATION_SPEED_VS_PERIOD_C + 0.5);
}

void MainWindow::setSimulationSpeed(int speed)
{
    _thread->setSimulationPeriod(gameSpeed2msec(speed));
}
