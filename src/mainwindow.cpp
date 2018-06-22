#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    network = new Network(false);
    handler = new Handler();
    connect(handler, &Handler::createSessionSuccess, this, &MainWindow::peerReceiverConnected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectToPeer_clicked()
{
    QJsonObject j;
    j["peerID"] = my_ipv6;
    j["action"] = "createSession";
    QString s = ui->peerID->text();
    network->sendDatagram(j, s);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(10000);
}

void MainWindow::slotTimerAlarm()
{
    if(receive)
    {
        timer->stop();
        receive = false;
    }
    else
    {
        int ret = QMessageBox::critical(this,QObject::tr("Error"),tr("Timeout Error"));
        timer->stop();
        delete timer;
    }

}

void MainWindow::peerReceiverConnected()
{
    receive = true;
    int ret = QMessageBox::information(this,QObject::tr("Info"),tr("Peer Available!"));
}