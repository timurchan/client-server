#ifndef TIMSERVER_H
#define TIMSERVER_H

#include <QTcpServer>

class TimServer : public QTcpServer
{
    Q_OBJECT

public:
    TimServer(QObject *parent=0);

private slots:
    void readyRead();
    void disconnected();
    void sendUserList();

protected:
    void incomingConnection(int socketfd);

private:
    typedef QMap<QTcpSocket*, QString> ClientInfoContainer;
    QMap<QTcpSocket*, QString> clients;
};

#endif // TIMSERVER_H
