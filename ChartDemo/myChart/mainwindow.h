#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mychart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow * ui;

    myChart * chart;
    QLineSeries * series;

    //QChart * chart;
    //QLineSeries * series;
    //QValueAxis * axisX;
    //QValueAxis * axisY;
};
#endif // MAINWINDOW_H
