#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartA(new DynamicChart())
    , chartG(new DynamicChart())
    , counter(0)
    , devPort(new MPU6050(1 << 14, this))
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    ui->cbox_PortInfo->addItems(MPU6050::AllPortsName());

    for(int i = 0; i < 3; ++i)
        chartA->addSeries(&seriesA[i]);
    chartA->setAnimationOptions(QChart::AnimationOption::NoAnimation);
    chartA->setTitle("Acce Chart");
    ui->view_Acce->setChart(chartA);

    for(int i = 0; i < 3; ++i)
        chartG->addSeries(&seriesG[i]);
    chartG->setAnimationOptions(QChart::AnimationOption::NoAnimation);
    chartG->setTitle("Gyro Chart");
    ui->view_Gyro->setChart(chartG);

    connect(timer, &QTimer::timeout, this, &MainWindow::PrintInfo);
}

MainWindow::~MainWindow()
{
    devPort->~MPU6050();
    delete devPort;
    delete timer;
    delete chartA;
    delete chartG;
    delete ui;
}

QString MainWindow::toQString(const qreal &val)
{
    return QString::fromStdString(std::to_string(val));
}

void MainWindow::on_cbox_PortInfo_currentTextChanged(const QString &portName)
{
    devPort->setPort(portName);
}


void MainWindow::on_pbtn_Clear_clicked()
{
    ui->ptxt_AcceMessage->clear();
    ui->ptxt_GyroMessage->clear();
}


void MainWindow::on_pbtn_Switch_clicked()
{
    if(ui->pbtn_Switch->text() == "Start") {
        ui->pbtn_Switch->setText("Stop");
        for(uint16_t i = 1 << 10; i; i >>= 1)
            if(devPort->open(QIODevice::ReadOnly)) break;
        devPort->clearError();

        timer->start(500); // 500ms pick one data is too slow but can't be fast than this.

    } else {
        ui->pbtn_Switch->setText("Start");
        devPort->clear();
        devPort->close();

        timer->stop();
    }
}

void MainWindow::PrintInfo() // too slow!
{
    QList<qreal> acceAngleArr = devPort->AcceAngleArr();
    QList<qreal> gyroAngleArr = devPort->GyroAngleArr();

    QString A_angleInfo = "A Angle { " + toQString(acceAngleArr[0]) + ", " + toQString(acceAngleArr[1]) + ", " + toQString(acceAngleArr[2]) + " }";
    QString G_angleInfo = "G Angle { " + toQString(gyroAngleArr[0]) + ", " + toQString(gyroAngleArr[1]) + ", " + toQString(gyroAngleArr[2]) + " }";

    ui->ptxt_AcceMessage->appendPlainText(A_angleInfo);
    ui->ptxt_GyroMessage->appendPlainText(G_angleInfo);

    for(int i = 0; i < 3; ++i)
    {
        chartA->pushPoint(QPointF(counter, acceAngleArr[i]), i);
        chartG->pushPoint(QPointF(counter, gyroAngleArr[i]), i);
    }
    counter++;
}

