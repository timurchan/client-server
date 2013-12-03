#include <QTcpSocket>
#include <QStringList>

#include "timtcpserver.h"
#include "xmlcommandsparser.h"

static QString prepareDataToSend(QString str) {
    str.replace("\n", "\t");
    str += '\n';
    return str;
}

TimTcpServer::TimTcpServer(QObject *parent) : QTcpServer(parent)
{
}

TimTcpServer::~TimTcpServer()
{
    bool q = 1;
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
        QString str = QString::fromUtf8(client->readLine()).trimmed();

        XMLCommandsParser parser(str);
        XMLCommandsParser::CommandsContainer commands = parser.getCommands();
        if(commands.find(XMLCommandsParser::CT_INIT) != commands.end()) {
            QString msg = QString("Server:" + clients[client] + " has joined.\n");
            XMLCommandsParser parser1(XMLCommandsParser::CT_MESSAGE, msg);
            QString str = prepareDataToSend(parser1.toString());

            for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                it.key()->write(str.toUtf8());
            }

            sendUserList();
        }
        if(commands.find(XMLCommandsParser::CT_MESSAGE) != commands.end()) {
            QString msg = clients[client] + " : " + parser.getMessage();
            XMLCommandsParser parser1(XMLCommandsParser::CT_MESSAGE, msg);
            QString str = prepareDataToSend(parser1.toString());

            for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if(it.key() != client) {
                    it.key()->write(str.toUtf8());
                }
            }

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
    XMLCommandsParser parser(XMLCommandsParser::CT_USERS, userList);
    QString str = prepareDataToSend(parser.toString());

    for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        it.key()->write(str.toUtf8());
    }
}
