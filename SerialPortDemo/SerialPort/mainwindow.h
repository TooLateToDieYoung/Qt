#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>

#include "mpu6050.h"

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
    void on_cbox_PortInfo_currentTextChanged(const QString &arg1);

    void on_pbtn_Clear_clicked();

    void on_pbtn_Switch_clicked();

    void PrintPortInfo();

private:
    QString static toQString(const qreal& val);

    Ui::MainWindow * ui;
    MPU6050 * devPort;
    QTimer * timer;
};
#endif // MAINWINDOW_H
