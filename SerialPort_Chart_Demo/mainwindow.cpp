#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartAcce(new DynamicChart())
    , chartAcceComponent(new DynamicChart())
    , chartAngleToAxis(new DynamicChart())
    , device(new MPU6050(1 << 13, this))
{
    ui->setupUi(this);

    InitCharts();

    ui->cbox_Port->addItems(MPU6050::AllPortsName());
    device->setPort(ui->cbox_Port->currentText());

    connect(device, &MPU6050::readSuccessful, this, &MainWindow::PrintInfo);
}

MainWindow::~MainWindow()
{
    device->~MPU6050();
    delete chartAcce;
    delete chartAcceComponent;
    delete chartAngleToAxis;
    delete device;
    delete ui;
}

void MainWindow::PrintInfo()
{
    disconnect(device, &MPU6050::readSuccessful, this, &MainWindow::PrintInfo);

    removeOldData();

    ChartWorker * componentWorker = new ChartWorkerComponent(chartAcceComponent, device);
    ChartWorker * angleWorker = new ChartWorkerAngles(chartAngleToAxis, device);
    ChartWorker * acceWorker = new ChartWorkerBasic(chartAcce, device);

    setTextBoxInfo();

    componentWorker->wait();
    angleWorker->wait();
    acceWorker->wait();

    connect(device, &MPU6050::readSuccessful, this, &MainWindow::PrintInfo);
}

void MainWindow::on_pbtn_Switch_clicked()
{
    if(ui->pbtn_Switch->text() == "Start") {
        ui->pbtn_Switch->setText("Stop");

        for(uint16_t i = 1 << 10; i; i >>= 1)
            if(device->open(QIODevice::ReadOnly)) break;
        device->clearError();

        connect(device, &MPU6050::readSuccessful, this, &MainWindow::PrintInfo);

    } else {
        ui->pbtn_Switch->setText("Start");
        device->clear();
        device->close();

        disconnect(device, &MPU6050::readSuccessful, this, &MainWindow::PrintInfo);
    }
}

void MainWindow::InitCharts()
{
    chartAcce->addSeries(&seriesAcce);
    chartAcce->setAxisYRange(5, 0, 1);
    ui->view_Acce->setChart(chartAcce);

    for(int i = 0; i < 3; ++i)
        chartAcceComponent->addSeries(&seriesAcceComponent[i]);
    chartAcceComponent->setAxisYRange(5, -5, 1);
    ui->view_AcceComponents->setChart(chartAcceComponent);

    for(int i = 0; i < 3; ++i)
        chartAngleToAxis->addSeries(&seriesAngleToAxis[i]);
    chartAngleToAxis->setAxisYRange(180, 0);
    ui->view_AngleToAxis->setChart(chartAngleToAxis);
}

void MainWindow::setTextBoxInfo()
{
    ui->ltxt_Acce_X->setText(QString::number(device->AcceComponent(0)));
    ui->ltxt_Acce_Y->setText(QString::number(device->AcceComponent(1)));
    ui->ltxt_Acce_Z->setText(QString::number(device->AcceComponent(2)));
    ui->ltxt_Angle_X->setText(QString::number(device->AngleToAxis(0)));
    ui->ltxt_Angle_Y->setText(QString::number(device->AngleToAxis(1)));
    ui->ltxt_Angle_Z->setText(QString::number(device->AngleToAxis(2)));
    ui->ltxt_Acce_Total->setText(QString::number(device->Acce()));
}

void MainWindow::removeOldData()
{
    seriesAcce.remove(0);
    for(int i = 0; i < 3; ++i)
    {
        seriesAcceComponent[i].remove(0);
        seriesAngleToAxis[i].remove(0);
    }
}
