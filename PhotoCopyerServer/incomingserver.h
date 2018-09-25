#ifndef INCOMINGSERVER_H
#define INCOMINGSERVER_H

#include <QTcpServer>

class Connection;

class IncomingServer : public QTcpServer
{
public:
    IncomingServer();
    virtual ~IncomingServer() override;
    void startListen();
    void stopListen();
protected:
    void incomingConnection(qintptr socketDescriptor) override;

protected:
    QVector<Connection*> unconfirmConnection;
    QVector<Connection*> confirmedConnection;
};

#endif // INCOMINGSERVER_H
