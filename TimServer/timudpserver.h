#ifndef TIMUDPSERVER_H
#define TIMUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTextStream>
#include <QString>
#include <QFile>
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
    explicit TimUdpServer(QFile& file, const QString& senderHost = "",
                          QObject *parent = 0);
    ~TimUdpServer();
    bool initSocket(int port);

private:

    void sendUserList();
    void sendMessage(const QString& message,
                     const Address& exceptAddress);
    void sendUdp(const Address& addr,
                 const QString &str);
    void log(const QString str);

private slots:
    void readPendingDatagrams();

private:
    QSet<Address> clients;
    QUdpSocket *udpSocket;
    QUdpSocket *udpOutSocket;
    QString allowedSenderHost;

    QTextStream stream;
};

#endif // TIMUDPSERVER_H
