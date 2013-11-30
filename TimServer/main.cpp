#include <QCoreApplication>
#include <QTextStream>
#include "timserver.h"

enum ProtocolType {
   UDP,
   TCP
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ProtocolType protocolType = TCP;
    int port = 4200;
    QString host = "@";

    if(protocolType == UDP) {

    } else if(protocolType == TCP) {
        TimServer *server = new TimServer();
        bool success = server->listen(QHostAddress::Any, port);
        if(!success)
        {
            qFatal("Could not listen on port 4200.");
        }
        qDebug() << "Ready";
    } else {
        QTextStream(stdout) << "Unknown protocol. Exiting...\n";
    }


    return a.exec();
}
