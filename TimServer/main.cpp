#include <QCoreApplication>
#include <QTextStream>
#include <QSharedPointer>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <getopt.h>

#include "timtcpserver.h"
#include "timudpserver.h"

const char * VERSION = "0.022";

enum ProtocolType {
   UDP,
   TCP,
   UNKNOWN
};

enum ErrorType {
    NO_ERROR,
    CMD_ERROR,
    COMMON_ERROR
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

bool parseHostString (const QString& hostString)
{
    if(hostString == "@") {
        return true;
    }

    if (hostString.isEmpty()) {
        return false;
    }

    const QStringList parts = hostString.split(".");
    if(parts.size() != 4) {
        return false;
    }

    foreach (const QString& part, parts) {
        bool ok = false;
        int port = part.toInt(&ok);
        if (!ok || port < 0) {
            return false;
        }
    }

    return true;
}

bool parseCmdString(const QString& cmdString, CmdInfoType& cmdInfo)
{
    // cmdString example: udp://192.168.9.122:4200
    //           example: udp://@:4200
    const QStringList dataList = cmdString.split(":");

    if (dataList.size() != 3) {
        QTextStream(stdout) << "Error in cmd string\n";
        return false;
    }

    ProtocolType protocol = getProtocolType(dataList[0]); // protocol type
    if (protocol == UNKNOWN) {
        QTextStream(stdout) << "Error in protocol type\n";
        return false;
    }

    QString hostSoFar = dataList[1];
    const QString slashes = hostSoFar.left(2);
    if (slashes != QString("//")) {
        QTextStream(stdout) << "Error in cmd string\n";
        return false;
    }

    hostSoFar.remove(0, 2);
    if(!parseHostString(hostSoFar)) {
        QTextStream(stdout) << "Error in host value\n";
        return false;
    }

    QString portSoFar = dataList[2];
    if (portSoFar.isEmpty()) {
        QTextStream(stdout) << "Error in port value\n";
        return false;
    }

    bool ok = false;
    int port = portSoFar.toInt(&ok);
    if (!ok || port < 0) {
        QTextStream(stdout) << "Error in port value\n";
        return false;
    }

    cmdInfo.host = hostSoFar;
    cmdInfo.port = port;
    cmdInfo.protocolType = protocol;

    return true;
}

const struct option longopts[] = {
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"cmd", required_argument, 0, 'c'},
    {"log", required_argument, 0, 'l'},
    {0, 0, 0, 0}
};

void printExamples() {
    QTextStream(stdout) << "Examples of cmd line:\n";
    QTextStream(stdout) << "\tudp://192.168.9.122:4200\n";
    QTextStream(stdout) << "\tudp://@:4200\n";
}

void printHelp(const QString& programName) {
    QTextStream(stdout) << programName << ":\n";
    QTextStream(stdout) << "\t--" << longopts[0].name
                        << ", -" << (char) longopts[0].val
                        << "\t\t\t\tprint this message and exit\n";
    QTextStream(stdout) << "\t--" << longopts[1].name
                        << ", -" << (char) longopts[1].val
                        << "\t\t\t\tprint version and exit\n";
    QTextStream(stdout) << "\t--" << longopts[2].name
                        << " <command>,  -" << (char) longopts[2].val
                        << " <command>\t\tset command string for starting\n";
    QTextStream(stdout) << "\t--" << longopts[3].name
                        << " <filename>, -" << (char) longopts[3].val
                        << " <filename>\t\tset filename for log\n";
    printExamples();
}

ErrorType parseInputArgs(int argc, char *argv[], CmdInfoType& cmdInfo)
{
    bool cmd = false;
    char ch;
    int longind;
    while ((ch = getopt_long(argc, argv, "?hvc:l:", longopts, &longind)) != -1) {
        switch (ch) {
            case '?':
            case 'h':
                printHelp(argv[0]);
                return NO_ERROR;
            case 'v':
                QTextStream(stdout) << argv[0] << " version " << VERSION << "\n";
                return NO_ERROR;
            case 'c':
                cmd = parseCmdString(QString(optarg), cmdInfo);
                if(!cmd) {
                    return CMD_ERROR;
                }
                break;
            case 'l':
                cmdInfo.logName = QString(optarg);
                break;
        }
    }

    return cmd ? NO_ERROR : COMMON_ERROR;
}

bool startServer(const CmdInfoType& cmdInfo)
{
    QString logName(cmdInfo.logName);
    if(logName.isEmpty()) {
        logName = "TimServer.log";  // default value;
    }
    static QFile logFile(logName);
    if ( !logFile.open(QIODevice::ReadWrite) ) {
        QTextStream(stdout) << "Can`t open log file\n";
        return false;
    }

    bool success = false;
    if(cmdInfo.protocolType == UDP) {
        QString allowedSenderHost = cmdInfo.host == "@" ? "" : cmdInfo.host;
        // можно не беспокоиться об освобождении памяти, оно произойдет при завершении программы
        TimUdpServer* server = new TimUdpServer(logFile, allowedSenderHost);
        success = server->initSocket(cmdInfo.port);

    } else if(cmdInfo.protocolType == TCP) {
        // можно не беспокоиться об освобождении памяти, оно произойдет при завершении программы
        TimTcpServer* server = new TimTcpServer(logFile);
        if(cmdInfo.host == "@") {
            success = server->listen(QHostAddress::Any, cmdInfo.port);
        } else {
            QHostAddress host(cmdInfo.host);
            success = server->listen(host, cmdInfo.port);
        }
        QTextStream(stdout) << "tcp server started : port = " << cmdInfo.port << "\n";
    } else {
        QTextStream(stdout) << "Unknown protocol. Exiting...\n";
        return false;
    }

    return success;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc == 1)
    {
        QTextStream(stdout) << "Arguments are absent. Exiting...\n";
        printHelp(argv[0]);
        return 4;
    }

    CmdInfoType cmdInfo;
    ErrorType result = parseInputArgs(argc, argv, cmdInfo);
    if(result != NO_ERROR)
    {
        QTextStream(stdout) << "Error while parsing cmd line. Exiting...\n";
        if(result == COMMON_ERROR) {
            printHelp(argv[0]);
        } else {
            printExamples();
        }
        return 1;
    }

    if(!startServer(cmdInfo))
    {
        QTextStream(stdout) << "Could not listen on port " << cmdInfo.port << ". Exiting...\n";
        return 2;
    }
    QTextStream(stdout) << "Ready...\n";

    int res = a.exec();
    return res;
}
