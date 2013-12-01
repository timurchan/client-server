#include <QTcpSocket>
#include <QStringList>

#include "timtcpserver.h"

TimTcpServer::TimTcpServer(QObject *parent) : QTcpServer(parent)
{
}

void TimTcpServer::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    QString user = client->peerAddress().toString() + ":" +
            QString::number(client->peerPort());
    clients[client] = user;

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void TimTcpServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line:" << line;

        QRegExp messageRegex("^/message:(.*)$");

        QString initString = "initString";
        if(line == initString)
        {
            for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                it.key()->write(QString("Server:" + it.value() + " has joined.\n").toUtf8());
            }
            sendUserList();
        }
        else if(messageRegex.indexIn(line) != -1)
        {
            QString message = line;
            QString newMessage = message.toUpper();
            QString user = clients[client];
            QTextStream(stdout) << user << ": " << message << " -> " << newMessage << "\n";
            client->write(QString(message + "\n").toUtf8());
        }
        else if(clients.contains(client))
        {
            QString message = line;
            QString user = clients[client];
            qDebug() << "User:" << user;
            qDebug() << "Message:" << message;

            for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                it.key()->write(QString(user + ":" + message + "\n").toUtf8());
            }
        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }
}

void TimTcpServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients.remove(client);

    sendUserList();
}

void TimTcpServer::sendUserList()
{
    QStringList userList(clients.values());

    for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        it.key()->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
    }
}
