#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartAcce(new DynamicChart())
    , chartAcceComponent(new DynamicChart())
    , chartAngleToAxis(new DynamicChart())
    , device(new MPU6050(MPU6050::AcceMode::G2, this))
{
    ui->setupUi(this);
    ui->cbox_Port->addItems(MPU6050::AllPortsName());
    device->setPort(ui->cbox_Port->currentText());
    InitCharts();
}

MainWindow::~MainWindow()
{
    device->~MPU6050();
    chartAcce->~DynamicChart();
    chartAcceComponent->~DynamicChart();
    chartAngleToAxis->~DynamicChart();

    delete chartAcce;
    delete chartAcceComponent;
    delete chartAngleToAxis;
    delete device;
    delete ui;
}

void MainWindow::PrintInfo() // spent 1 ~ 3 ms
{   
    disconnect(device, &MPU6050::readSuccessful, this, &MainWindow::PrintInfo);

    ChartWorker * componentWorker = new ChartWorkerComponent(chartAcceComponent, device);
    ChartWorker * angleWorker = new ChartWorkerAngles(chartAngleToAxis, device);
    ChartWorker * acceWorker = new ChartWorkerBasic(chartAcce, device);

    QFuture<void> printString = QtConcurrent::run(this, &MainWindow::setTextBoxInfo);

    componentWorker->wait();
    angleWorker->wait();
    acceWorker->wait();
    printString.waitForFinished();

    connect(device, &MPU6050::readSuccessful, this, &MainWindow::PrintInfo);
}

void MainWindow::on_pbtn_Switch_clicked()
{
    if(ui->pbtn_Switch->text() == "Start") {
        ui->pbtn_Switch->setText("Stop");
        if(!device->open(QIODevice::ReadOnly)) throw "Port Not Open";
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
    chartAcce->addSomeEmptySeries(1);
    chartAcce->setAxisYRange(2, 0, 3);
    ui->view_Acce->setChart(chartAcce);

    chartAcceComponent->addSomeEmptySeries(3);
    chartAcceComponent->setAxisYRange(2, -2, 5);
    ui->view_AcceComponents->setChart(chartAcceComponent);

    chartAngleToAxis->addSomeEmptySeries(3);
    chartAngleToAxis->setAxisYRange(180, 0, 5);
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
