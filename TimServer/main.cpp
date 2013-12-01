#include <QCoreApplication>
#include <QTextStream>
#include <QSharedPointer>
#include <getopt.h>

#include "timtcpserver.h"
#include "timudpserver.h"

const char * VERSION = "0.01";

enum ProtocolType {
   UDP,
   TCP
};

struct CmdInfoType {
    QString host;
    int port;
    ProtocolType protocolType;
    QString logName;
};

bool parseCmdString(const QString& cmdString, CmdInfoType& cmdInfo)
{
    ProtocolType protocolType = UDP;
    int port = 5300;
    QString host = "@";

    cmdInfo.host = host;
    cmdInfo.port = port;
    cmdInfo.protocolType = protocolType;

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
        TimUdpServer* server = new TimUdpServer();
        success = server->initSocket();

    } else if(cmdInfo.protocolType == TCP) {
        //QSharedPointer<TimTcpServer> server = QSharedPointer<TimTcpServer>(new TimTcpServer());
        TimTcpServer* server = new TimTcpServer();
        success = server->listen(QHostAddress::Any, cmdInfo.port);
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
