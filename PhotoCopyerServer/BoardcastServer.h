#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QVector>

class QTimer;
class Connection;
class QUdpSocket;

class BoardcastServer : public QObject
{
    Q_OBJECT
public:
    BoardcastServer(QObject *parent = nullptr);
    virtual ~BoardcastServer();
signals:
//    void newConnection(Connection *connection);

protected:
//    void incomingConnection(qintptr socketDescriptor) override;

protected slots:
    void readBroadcastDatagram();

private:
    QTimer *requestTimer;
    QUdpSocket *broadcastSocket;
    QHostAddress localIp;
};

#endif // SERVER_H
