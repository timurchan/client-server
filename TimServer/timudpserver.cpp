#include <QStringList>
#include "timudpserver.h"

TimUdpServer::TimUdpServer(QObject *parent) :
    QObject(parent)
{
}

bool TimUdpServer::initSocket()
{
    udpSocket = new QUdpSocket(this);
    bool isBind = udpSocket->bind(QHostAddress::LocalHost, 4300);
    if(isBind)
    {
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        qDebug() << "udp server started : " << udpSocket->localAddress()
                 << ", port : " << udpSocket->localPort();
    }
    return isBind;
}

void TimUdpServer::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        qDebug() << "sender ip : " << sender.toString();
    }
}

void TimUdpServer::sendUserList()
{
    QStringList userList(clients.values());

    for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        it.key()->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
    }
}
