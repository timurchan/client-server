#include <QTcpSocket>
#include <QStringList>

#include "timtcpserver.h"
#include "xmlcommandsparser.h"

static QString prepareDataToSend(QString str) {
    str.replace("\n", "\t");
    str += '\n';
    return str;
}

TimTcpServer::TimTcpServer(QFile& file, QObject *parent) : QTcpServer(parent)
{
    stream.setDevice(&file);
}

TimTcpServer::~TimTcpServer()
{
}

void TimTcpServer::log(const QString str)
{
    QTextStream(stdout) << str << "\n";
    stream << str << "\n";
    stream.flush();
}

void TimTcpServer::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    QString user = client->peerAddress().toString() + ":" +
            QString::number(client->peerPort());
    clients[client] = user;

    log("New client from: " + client->peerAddress().toString()
                           + QString::number(client->peerPort()));

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
            QString msg = QString("New client from: " + clients[client]);
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
            log(msg);
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
    QString disconnect_msg = "Client disconnected: " + client->peerAddress().toString()
                              + QString::number(client->peerPort());
    log(disconnect_msg);
    clients.remove(client);

    XMLCommandsParser parser1(XMLCommandsParser::CT_MESSAGE, disconnect_msg);
    QString str = prepareDataToSend(parser1.toString());

    for(ClientInfoContainer::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        it.key()->write(str.toUtf8());
    }

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
