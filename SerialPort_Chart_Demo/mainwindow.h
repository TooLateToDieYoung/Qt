#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include "mpu6050.h"
#include "dynamicchart.h"
#include "chartworker.h"

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
    void on_pbtn_Switch_clicked();

private:
    void InitCharts();
    void PrintInfo();
    void setTextBoxInfo();
    void removeOldData();

    Ui::MainWindow * ui;

    DynamicChart * chartAcce;
    QLineSeries seriesAcce;

    DynamicChart * chartAcceComponent;
    QLineSeries seriesAcceComponent[3];

    DynamicChart * chartAngleToAxis;
    QLineSeries seriesAngleToAxis[3];

    MPU6050 * device;
};
#endif // MAINWINDOW_H
