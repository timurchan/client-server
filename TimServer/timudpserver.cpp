#include <QStringList>
#include "timudpserver.h"
#include "xmlcommandsparser.h"

uint qHash(const Address& addr) {
    return qHash(addr.host) * qHash(addr.port);
}


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

            int port = parser.getId(); // не тот, с которого шлет клиент, а тот, который он слушает
            clients.insert(Address(senderHostStr, port));
            sendUserList();
        }
        if(commands.find(XMLCommandsParser::CT_MESSAGE) != commands.end()) {

            // это пока никак не реализовать, потому что неизвестно, от кого сообщение
            // senderPort - каждый раз новый, на него нельзя ориентироваться
            // хотел ориентироваться на int port = parser.getPort() -
            // но про него пока можно узнать лишь в случае CT_INIT. Надо додумать

            QString in_msg = parser.getMessage();
            int id = parser.getId();
            QString who = senderHostStr + ":" + QString::number(id);
            QString out_msg = who + " : " + in_msg;
            sendMessage(out_msg, Address(senderHostStr, id));
        }

    }
}

void TimUdpServer::sendUserList()
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
        udpOutSocket->writeDatagram(data, QHostAddress(client.host), quint16(client.port));
    }
}

void TimUdpServer::sendMessage(const QString &message,
                               const Address& exceptAddress)
{
    foreach (const Address& client, clients) {
        if(! (client == exceptAddress)) {
            QUdpSocket* udpOutSocket = new QUdpSocket(this);

            XMLCommandsParser parser(XMLCommandsParser::CT_MESSAGE, message);
            QString str = parser.toString();

            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out << qint16(0);
            out << str;
            out.device()->seek(qint16(0));
            int sz = data.size();
            out << qint16(data.size() - sizeof(qint16));
            udpOutSocket->writeDatagram(data, QHostAddress(client.host), quint16(client.port));
        }
    }
}
