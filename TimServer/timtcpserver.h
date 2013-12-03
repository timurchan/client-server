#ifndef TIMSERVER_H
#define TIMSERVER_H

#include <QTcpServer>
#include <QTextStream>
#include <QFile>

class TimTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TimTcpServer(QFile& file, QObject *parent=0);
    ~TimTcpServer();

private slots:
    void readyRead();
    void disconnected();

protected:
    void incomingConnection(int socketfd);

private:
    void sendUserList();
    void log(const QString str);

private:
    typedef QMap<QTcpSocket*, QString> ClientInfoContainer;
    QMap<QTcpSocket*, QString> clients;

    QTextStream stream;
};

#endif // TIMSERVER_H
