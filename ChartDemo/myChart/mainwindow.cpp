#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chart(new myChart())
{
    ui->setupUi(this);

    // <Hint>
    // (1) get datas outside
    // (2) myChart class just plot those datas
    // (3) view widget belongs to ui ( not a work of myChart )

    series = new QLineSeries();
    for(auto i = 0; i < 100; ++i)
        *series << QPointF(i, rand() % 100 - 50); // (1)

    chart->addSeries(series);  // (2)
    ui->view->setChart(chart); // (3)
    ui->view->setRenderHint(QPainter::Antialiasing);

/*
 * hide the following operation in myChart class
 *
    chart  = new QChart();
    series = new QLineSeries();
    series->setPointsVisible(false);
    chart->removeAllSeries();
    series->clear();

    axisX = new QValueAxis();
    axisX->setTitleText("X-Axis");
    axisX->setRange(0, 100);
    axisX->setTickCount(11);
    axisX->setMinorTickCount(2);
    axisX->setLabelFormat("%d");
    axisX->setGridLineVisible(false);
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setTitleText("Y-Axis");
    axisY->setRange(-50, 50);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(5);
    axisY->setLabelFormat("%d");
    axisY->setGridLineVisible(true);
    chart->addAxis(axisY, Qt::AlignLeft);

    for(auto i = 0; i < 100; ++i)
        *series << QPointF(i, rand() % 100 - 50);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->setTitle("Test of Axis X and Y");

    ui->view->setChart(chart);
    ui->view->setRenderHint(QPainter::Antialiasing);
*/
}

MainWindow::~MainWindow()
{
    chart->~myChart(); // calling distructor to release memory
    delete chart;
    delete series;

    //delete chart;
    //delete series;
    //delete axisX;
    //delete axisY;

    delete ui;
}

