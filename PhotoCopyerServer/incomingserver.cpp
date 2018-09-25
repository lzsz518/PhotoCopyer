#include <QtNetwork>
#include "connection.h"
#include "incomingserver.h"

#define LISTENPORT 65000

IncomingServer::IncomingServer()
{

}

IncomingServer::~IncomingServer()
{

}

void IncomingServer::startListen()
{
    listen(QHostAddress::Any,LISTENPORT);
}

void IncomingServer::stopListen()
{
    close();
}

void IncomingServer::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(this);
    if(connection!=nullptr)
    {
        connection->setSocketDescriptor(socketDescriptor);
        confirmedConnection.append(connection);
        connection->sendMessage("Hello Socket!");
    }

}
