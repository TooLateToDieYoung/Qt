#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , devPort(new MPU6050(1 << 14, this))
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    ui->cbox_PortInfo->addItems(MPU6050::AllPortsName());

    connect(timer, &QTimer::timeout, this, &MainWindow::PrintPortInfo);
}

MainWindow::~MainWindow()
{
    devPort->~MPU6050();
    delete devPort;
    delete timer;
    delete ui;
}


void MainWindow::on_cbox_PortInfo_currentTextChanged(const QString &portName)
{
    devPort->setPort(portName);
}


void MainWindow::on_pbtn_Clear_clicked()
{
    ui->ptxt_A_AngleMessage->clear();
    ui->ptxt_G_AngleMessage->clear();
}


void MainWindow::on_pbtn_Switch_clicked()
{
    if(ui->pbtn_Switch->text() == "Start")
    {
        ui->pbtn_Switch->setText("Stop");
        for(uint16_t i = 1 << 10; i; i >>= 1)
            if(devPort->open(QIODevice::ReadOnly)) break;
        devPort->clearError();

        timer->start(50);
    }
    else
    {
        ui->pbtn_Switch->setText("Start");
        devPort->clear();
        devPort->close();

        timer->stop();
    }
}

void MainWindow::PrintPortInfo()
{
    QList<qreal> acceAngleArr = devPort->AcceAngleArr();
    QList<qreal> gyroAngleArr = devPort->GyroAngleArr();

    QString A_angleInfo = "A Angle { " + toQString(acceAngleArr[0]) + ", " + toQString(acceAngleArr[1]) + ", " + toQString(acceAngleArr[2]) + " }";
    QString G_angleInfo = "G Angle { " + toQString(gyroAngleArr[0]) + ", " + toQString(gyroAngleArr[1]) + ", " + toQString(gyroAngleArr[2]) + " }";

    ui->ptxt_A_AngleMessage->appendPlainText(A_angleInfo);
    ui->ptxt_G_AngleMessage->appendPlainText(G_angleInfo);
}

QString MainWindow::toQString(const qreal &val)
{
    return QString::fromStdString(std::to_string(val));
}

