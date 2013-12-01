#ifndef TIMUDPSERVER_H
#define TIMUDPSERVER_H

#include <QObject>
#include <QUdpSocket>

class TimUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit TimUdpServer(QObject *parent = 0);

    bool initSocket();
    void sendUserList();

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket* udpSocket;

    typedef QMap<QUdpSocket*, QString> ClientInfoContainer;
    QMap<QUdpSocket*, QString> clients;
};

#endif // TIMUDPSERVER_H
