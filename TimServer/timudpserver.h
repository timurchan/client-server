#ifndef TIMUDPSERVER_H
#define TIMUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QVector>

class TimUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit TimUdpServer(QObject *parent = 0);

    bool initSocket();

private:
    struct Address {
        Address() {}
        Address(const QString& host_, int port_) :
            host(host_),
            port(port_)
        {}

        QString host;
        int port;
    };

private:

    void sendUserList();
    void sendMessage(const QString& message,
                     const Address& exceptAddress);

private slots:
    void readPendingDatagrams();

private:
    QVector<Address> clients;
    QUdpSocket *udpSocket;
};

#endif // TIMUDPSERVER_H
