#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include "dynamicchart.h"

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_trigger_clicked();

    void pushRandPoint();

private:
    Ui::MainWindow * ui;
    DynamicChart * chart;
    QLineSeries * seriesA;
    QLineSeries * seriesG;
    int counter;

    QTimer * timer;
};
#endif // MAINWINDOW_H
