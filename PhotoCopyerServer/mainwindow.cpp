#include "incomingserver.h"
#include "BoardcastServer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bcServer = new BoardcastServer;
    inServer = new IncomingServer;
    inServer->startListen();
}

MainWindow::~MainWindow()
{
    if(bcServer!=nullptr)
        delete bcServer;
    if(inServer!=nullptr)
    {
        inServer->stopListen();
        delete inServer;
    }
    delete ui;
}
