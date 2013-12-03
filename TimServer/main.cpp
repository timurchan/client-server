#include <QCoreApplication>
#include <QTextStream>
#include <QSharedPointer>
#include <QStringList>
#include <getopt.h>

#include "timtcpserver.h"
#include "timudpserver.h"

const char * VERSION = "0.022";

enum ProtocolType {
   UDP,
   TCP,
   UNKNOWN
};

struct CmdInfoType {
    QString host;
    int port;
    ProtocolType protocolType;
    QString logName;
};

ProtocolType getProtocolType(const QString& str)
{
    const QString typeS = str.toUpper();
    if (typeS == QString("UDP"))
        return UDP;
    else if (typeS == QString("TCP"))
        return TCP;
    else
        return UNKNOWN;
}

bool parseCmdString(const QString& cmdString, CmdInfoType& cmdInfo)
{
    // cmdString example: udp://192.168.9.122:4200
    //           example: udp://@:4200
    const QStringList dataList = cmdString.split(":");

    if (dataList.size() != 3)
        return false;

    ProtocolType protocol = getProtocolType(dataList[0]); // protocol type
    if (protocol == UNKNOWN)
        return false;

    QString hostSoFar = dataList[1];
    const QString slashes = hostSoFar.left(2);
    if (slashes != QString("//"))
        return false;
    else
        hostSoFar.remove(0, 2);

    QString portSoFar = dataList[2];
    if (portSoFar.isEmpty())
        return false;

    bool ok = false;
    int port = portSoFar.toInt(&ok);
    if (!ok || port < 0)
        return false;

    cmdInfo.host = hostSoFar;
    cmdInfo.port = port;
    cmdInfo.protocolType = protocol;

    return true;
}

bool parseInputArgs(int argc, char *argv[], CmdInfoType& cmdInfo)
{
    const struct option longopts[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"cmd", required_argument, 0, 'c'},
        {"log", required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };
    char ch;
    int longind;
    while ((ch = getopt_long(argc, argv, "?hvc:l:", longopts, &longind)) != -1) {
        switch (ch) {
            case '?':
            case 'h':
                QTextStream(stdout) << argv[0] << ":\n";
                QTextStream(stdout) << "\t--" << longopts[0].name
                                    << ", -" << (char) longopts[0].val
                                    << "\t\t\t\tprint this message and exit\n";
                QTextStream(stdout) << "\t--" << longopts[1].name
                                    << ", -" << (char) longopts[1].val
                                    << "\t\t\t\tprint version and exit\n";
                QTextStream(stdout) << "\t--" << longopts[2].name
                                    << " <command>, -" << (char) longopts[2].val
                                    << " <command>\t\tset command string for starting\n";
                QTextStream(stdout) << "\t--" << longopts[3].name
                                    << " <filename>, -" << (char) longopts[3].val
                                    << "<filename>\t\tset filename for log\n";
                return false;
            case 'v':
                QTextStream(stdout) << argv[0] << " version " << VERSION << "\n";
                return false;
            case 'c':
                if(!parseCmdString(QString(optarg), cmdInfo))
                {
                    return false;
                }
                break;
            case 'l':
                cmdInfo.logName = QString(optarg);
                break;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc == 1)
    {
        QTextStream(stdout) << "Arguments are absent. Exiting...\n";
        return 4;
    }

    CmdInfoType cmdInfo;
    if(!parseInputArgs(argc, argv, cmdInfo))
    {
        QTextStream(stdout) << "Error while parsing cmd line. Exiting...";
        return 1;
    }

    bool success = false;

    if(cmdInfo.protocolType == UDP) {
        //QSharedPointer<TimUdpServer> server = QSharedPointer<TimUdpServer>(new TimUdpServer());
        QString allowedSenderHost = (cmdInfo.host == "@") ? "" : cmdInfo.host;
        TimUdpServer* server = new TimUdpServer(allowedSenderHost);
        success = server->initSocket(cmdInfo.port);

    } else if(cmdInfo.protocolType == TCP) {
        //QSharedPointer<TimTcpServer> server = QSharedPointer<TimTcpServer>(new TimTcpServer());
        TimTcpServer* server = new TimTcpServer();
        if(cmdInfo.host == "@") {
            success = server->listen(QHostAddress::Any, cmdInfo.port);
        } else {
            QHostAddress host(cmdInfo.host);
            success = server->listen(host, cmdInfo.port);
        }
        qDebug() << "tcp server started : port = " << cmdInfo.port;
    } else {
        QTextStream(stdout) << "Unknown protocol. Exiting...\n";
        return 3;
    }

    if(!success)
    {
        QTextStream(stdout) << "Could not listen on port " << cmdInfo.port << ". Exiting...\n";
        return 2;
    }
    QTextStream(stdout) << "Ready.\n";

    int res = a.exec();
    return res;
}
