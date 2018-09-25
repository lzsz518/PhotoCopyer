#include <QTimer>
#include <QtNetwork>
#include "connection.h"
#include "BoardcastServer.h"

static const qint32 BroadcastInterval = 2000;
static const unsigned broadcastPort = 45000;

BoardcastServer::BoardcastServer(QObject *parent)
{
    requestTimer = new QTimer;
    broadcastSocket = new QUdpSocket;
    broadcastSocket->bind(QHostAddress::Any,broadcastPort,QUdpSocket::ShareAddress|
                          QUdpSocket::ReuseAddressHint);
    connect(broadcastSocket,SIGNAL(readyRead()),this,SLOT(readBroadcastDatagram()));

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            localIp = address;
    }
}

BoardcastServer::~BoardcastServer()
{
    if(requestTimer!=nullptr)
    {
        if(requestTimer->isActive())
            requestTimer->stop();
        delete requestTimer;
    }
    if(broadcastSocket!=nullptr)
        delete broadcastSocket;

}

//void BoardcastServer::incomingConnection(qintptr socketDescriptor)
//{
//    Connection *connection = new Connection(this);
//    connection->setSocketDescriptor(socketDescriptor);
//    QString hostIpPort = serverAddress().toString()+QString().arg(serverPort());
//    QByteArray datagram(hostIpPort.toStdString().c_str());
//    connection->write(datagram);
//}

void BoardcastServer::readBroadcastDatagram()
{
    while (broadcastSocket->hasPendingDatagrams())
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(broadcastSocket->pendingDatagramSize());
        if (broadcastSocket->readDatagram(datagram.data(), datagram.size(),
                                         &senderIp, &senderPort) == -1)
            continue;

        QList<QByteArray> list = datagram.split('@');
        if (list.size() != 3)
            continue;
        QString senderIpStr(list.at(2).toStdString().c_str());

        int senderBoardcastServerPort = list.at(1).toInt();

        senderIp.setAddress(senderIpStr);
        datagram.clear();
        datagram.append(localIp.toString().toUtf8());
        datagram.append("@");
        datagram.append(QByteArray::number(broadcastPort));
        datagram.append("@");
        datagram.append("Welcome!");
        broadcastSocket->writeDatagram(datagram,senderIp,broadcastPort);
    }

}
