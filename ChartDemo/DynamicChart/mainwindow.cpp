#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chart(new DynamicChart())
    , seriesA(new QLineSeries())
    , seriesG(new QLineSeries())
    , counter(0)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    chart->addSeries(seriesA);
    chart->addSeries(seriesG);
    chart->setAnimationOptions(QChart::AnimationOption::NoAnimation);

    ui->view->setChart(chart);
    ui->view->setRenderHint(QPainter::Antialiasing);

    connect(timer, SIGNAL(timeout()), this, SLOT(pushRandPoint()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete seriesA;
    delete seriesG;
    delete chart;
    delete ui;
}


void MainWindow::on_trigger_clicked()
{
    pushRandPoint();
}

void MainWindow::pushRandPoint()
{
    chart->pushPoint(QPointF(counter, rand() % 100 - 50), 0);
    chart->pushPoint(QPointF(counter, rand() % 100 - 50), 1);
    counter++;
}

