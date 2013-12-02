#include <QStringList>
#include "timudpserver.h"
#include "xmlcommandsparser.h"

TimUdpServer::TimUdpServer(QObject *parent) :
    QObject(parent)
{
}

bool TimUdpServer::initSocket()
{
    udpSocket = new QUdpSocket(this);
    bool isBind = udpSocket->bind(QHostAddress::LocalHost, 4200);
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

    QTextStream(stdout) << "Datagram reveiced.\n";
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress senderHost;
        quint16 senderPort;

        QDataStream in(&datagram, QIODevice::ReadOnly);

        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderHost, &senderPort);

        qint16 sz = 0;
        QString str;
        in >> sz;
        in >> str;

        QString senderHostStr = senderHost.toString();
        qDebug() << "sender ip : " << senderHostStr;
        qDebug() << "sender port : " << senderPort;

        //QString str(datagram);
        XMLCommandsParser parser(str);

        XMLCommandsParser::CommandsContainer commands = parser.getCommands();
        if(commands.find(XMLCommandsParser::CT_INIT) != commands.end()) {

            clients.push_back(Address(senderHostStr, senderPort));
            sendUserList(senderHostStr, 7755);
        }

    }
}

void TimUdpServer::sendUserList(const QString& host, int port)
{
    QStringList clientNames;
    foreach (const Address& client, clients) {
        clientNames << (client.host + ":" + QString::number(client.port));
    }

    foreach (const Address& client, clients) {
        QUdpSocket* udpOutSocket = new QUdpSocket(this);

        XMLCommandsParser parser(XMLCommandsParser::CT_USERS, clientNames);
        QString str = parser.toString();

        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << qint16(0);
        out << str;
        out.device()->seek(qint16(0));
        int sz = data.size();
        out << qint16(data.size() - sizeof(qint16));
        udpOutSocket->writeDatagram(data, QHostAddress(host), quint16(port));
    }
}
