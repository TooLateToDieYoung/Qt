#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTimer>

#include "mpu6050.h"
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
    void on_cbox_PortInfo_currentTextChanged(const QString &portName);

    void on_pbtn_Clear_clicked();

    void on_pbtn_Switch_clicked();

    void PrintInfo();

private:
    QString static toQString(const qreal &val);

    Ui::MainWindow * ui;

    DynamicChart * chartA;
    QLineSeries seriesA[3];

    DynamicChart * chartG;
    QLineSeries seriesG[3];

    int counter;

    MPU6050 * devPort;
    QTimer * timer;

};
#endif // MAINWINDOW_H
