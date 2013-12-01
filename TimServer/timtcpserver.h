#ifndef TIMSERVER_H
#define TIMSERVER_H

#include <QTcpServer>

class TimTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TimTcpServer(QObject *parent=0);

private slots:
    void readyRead();
    void disconnected();

protected:
    void incomingConnection(int socketfd);

private:
    void sendUserList();

private:
    typedef QMap<QTcpSocket*, QString> ClientInfoContainer;
    QMap<QTcpSocket*, QString> clients;
};

#endif // TIMSERVER_H
