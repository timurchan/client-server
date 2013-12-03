#ifndef TIMUDPSERVER_H
#define TIMUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QSet>

struct Address {
    Address() {}
    Address(const QString& host_, int port_) :
        host(host_),
        port(port_)
    {}

    bool operator ==(const Address& other) const {
        return host == other.host && port == other.port;
    }

    QString host;
    int port;
};



class TimUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit TimUdpServer(QObject *parent = 0);

    bool initSocket();

private:

    void sendUserList();
    void sendMessage(const QString& message,
                     const Address& exceptAddress);

private slots:
    void readPendingDatagrams();

private:
    QSet<Address> clients;
    QUdpSocket *udpSocket;
};

#endif // TIMUDPSERVER_H
