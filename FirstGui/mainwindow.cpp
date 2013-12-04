#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xmlcommandsparser.h"

const QString MainWindow::DEFAULT_HOST = QString("127.0.0.1");
const int     MainWindow::DEFAULT_PORT = 5300;
const QString MainWindow::SERVER_COLOR = QString("red");
const QString MainWindow::CLIENT_COLOR = QString("blue");
const int     MainWindow::DEFAULT_PORT_CLIENT = 7757;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_host(DEFAULT_HOST),
    m_protocol_type(MainWindow::TCP),
    m_portHost(DEFAULT_PORT),
    m_clientPort(DEFAULT_PORT_CLIENT),
    m_isBindUdpInSocket(false),
    socketTcp(0),
    socketUdp(0),
    udpOutSocket()
{
    ui->setupUi(this);

    enableTuneUpWidgets(true);
    ui->actionDisconnect->setEnabled(false);
    enableChatWidgets(false);

    ui->dialogW->setReadOnly(true);
    init_server_connection();

    //connect(tuneUpUi->applyPushButton, SIGNAL(clicked()), this, SLOT(onClickApplyTuneButton()));

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::getNewSettings()
{
    m_host = ui->srvHostLE->text();

    bool ok = false;
    const int portHostSoFar = ui->srvPortLE->text().toInt(&ok);
    m_portHost = (ok && portHostSoFar > 0) ? portHostSoFar : MainWindow::DEFAULT_PORT;

    const int idxCB = ui->clCB->currentIndex();
    m_protocol_type = (idxCB == 0) ? MainWindow::TCP : MainWindow::UDP;

    if (m_protocol_type == MainWindow::UDP)
    {
        bool ok = false;
        const int portClientSoFar = ui->srvPortLE->text().toInt(&ok);
        m_clientPort = (ok && portClientSoFar > 0) ? portClientSoFar : MainWindow::DEFAULT_PORT_CLIENT;
    }
}

void MainWindow::on_actionServerConnect_triggered()
{
    getNewSettings();

    ui->statusBar->showMessage(QString("Trying to connect to the server..."));

    if(m_protocol_type == UDP)
    {
        socketUdp = new QUdpSocket(this);
        if(!m_isBindUdpInSocket)
        {
            m_isBindUdpInSocket = socketUdp->bind(QHostAddress::LocalHost, m_clientPort); // listen from server
            enableChatWidgets(true);
            enableTuneUpWidgets(false);
            ui->actionServerConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
        }
        if(m_isBindUdpInSocket)
        {
            connect(socketUdp, SIGNAL(readyRead()), this, SLOT(readyReadUdp()));

            udpOutSocket = new QUdpSocket(this);
            connectedUdp();

            ui->statusBar->showMessage(QString("server ip: ") + m_host +
                                       QString(", port: ") + QString::number((int)m_portHost));
            enableChatWidgets(true);
            enableTuneUpWidgets(false);
            ui->actionServerConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
        }
        else
        {
            ui->statusBar->showMessage("Bind local port for listen server fail.");
            enableChatWidgets(false);
            enableTuneUpWidgets(true);
            ui->actionServerConnect->setEnabled(true);
            ui->actionDisconnect->setEnabled(false);
        }

    }
    else // TCP
    {
        delete socketTcp;
        ui->clientsWidget->clear();
        ui->dialogW->clear();

        socketTcp = new QTcpSocket(this);
        connect(socketTcp, SIGNAL(readyRead()), this, SLOT(readyReadTcp()));
        connect(socketTcp, SIGNAL(connected()), this, SLOT(connectedTcp()));
        socketTcp->connectToHost(m_host, m_portHost);

        ui->statusBar->showMessage(QString("New connect to server ip: ") + m_host +
                                   QString(", port: ") + QString::number((int)m_portHost));

        enableChatWidgets(true);
        enableTuneUpWidgets(false);

    }
}

void MainWindow::on_actionExit_triggered()
{
    ui->statusBar->showMessage(QString("Close connection"));
    this->close();
}

void MainWindow::init_server_connection()
{
    ui->srvHostLE->setText(m_host);
    ui->srvPortLE->setText(QString::number(m_portHost));
    ui->clCB->setCurrentIndex(m_protocol_type);
    if (m_clientPort == UDP)
    {
        ui->clPortL->setEnabled(true);
        ui->clPortLE->setEnabled(true);
        ui->clPortLE->setText(QString::number(m_clientPort));
    }
    else
    {
        ui->clPortL->setEnabled(false);
        ui->clPortLE->setEnabled(false);
        ui->clPortLE->clear();
    }


//    tuneUpUi->hostLineEdit->setText(m_host);
//    tuneUpUi->portLineEdit->setText(QString::number(m_portHost));
//    tuneUpUi->protocolComboBox->setCurrentIndex(m_protocol_type);
}

void MainWindow::appendDataToDialog(MainWindow::User user_type, const QString &str)
{
    QString user_name = QString("unknown: ");
    const QColor default_color = ui->dialogW->textColor();
    QColor user_color = default_color;
    switch (user_type)
    {
    case MainWindow::SERVER:
        user_name = QString("server: ");
        user_color = QColor(MainWindow::SERVER_COLOR);
        break;
    case MainWindow::CLIENT:
        user_name = QString("me: ");
        user_color = QColor(MainWindow::CLIENT_COLOR);
        break;
    }
    ui->dialogW->setTextColor(user_color);
    ui->dialogW->insertPlainText(user_name); //textCursor().insertText(text);
    //ui->dialogW->append(user_name);
    ui->dialogW->setTextColor(default_color);
    ui->dialogW->insertPlainText(str + QString("\n"));
    //ui->dialogW->append(str);
    //ui->dialogW->append(QString());
}

void MainWindow::on_messageLineEdit_returnPressed()
{
    QString text = ui->messageLineEdit->text();
    if (text.isEmpty())
        return;

    appendDataToDialog(MainWindow::CLIENT, text);

    if(m_protocol_type == UDP) {
        XMLCommandsParser parser(XMLCommandsParser::CT_MESSAGE, text);
        parser.addCommand(XMLCommandsParser::CT_ID, m_clientPort);
        sendUdp(parser.toString());
    } else { // TCP
        XMLCommandsParser parser(XMLCommandsParser::CT_MESSAGE, text);
        sendTcp(parser.toString());
    }

    ui->messageLineEdit->clear();
}

void MainWindow::showUsers(const QStringList &users)
{
    for(int i = 0; i < userIcons.size(); i++)
    {
        delete userIcons[i];
    }
    userIcons.clear();
    ui->clientsWidget->clear();
    ui->clientsWidget->setIconSize(QSize(24, 24));
    foreach(QString user, users)
    {
        userIcons.push_back(new QListWidgetItem(QPixmap("user.png"), user, ui->clientsWidget));
    }
}

void MainWindow::readyReadTcp()
{
    while(socketTcp->canReadLine())
    {
        QString str = QString::fromUtf8(socketTcp->readLine()).trimmed();

        XMLCommandsParser parser(str);
        XMLCommandsParser::CommandsContainer commands = parser.getCommands();
        if(commands.find(XMLCommandsParser::CT_USERS) != commands.end()) {
            QStringList users = parser.getUsers();
            showUsers(users);
        }
        if(commands.find(XMLCommandsParser::CT_MESSAGE) != commands.end()) {
            const QString text = parser.getMessage();
            if (text.isEmpty())
                return;
            appendDataToDialog(MainWindow::SERVER, text);
        }
    }
}

void MainWindow::readyReadUdp()
{
    QTextStream(stdout) << "Datagram reveiced.\n";
    while (socketUdp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketUdp->pendingDatagramSize());
        QHostAddress senderHost;
        quint16 senderPort;

        QDataStream in(&datagram, QIODevice::ReadOnly);

        socketUdp->readDatagram(datagram.data(), datagram.size(), &senderHost, &senderPort);

        qint16 sz = 0;
        QString str;
        in >> sz;
        in >> str;

        QString senderHostStr = senderHost.toString();
        qDebug() << "sender ip : " << senderHostStr;
        qDebug() << "sender port : " << senderPort;

        XMLCommandsParser parser(str);

        XMLCommandsParser::CommandsContainer commands = parser.getCommands();
        if(commands.find(XMLCommandsParser::CT_USERS) != commands.end()) {
            QStringList users = parser.getUsers();
            showUsers(users);
        }
        if(commands.find(XMLCommandsParser::CT_MESSAGE) != commands.end()) {
            const QString text = parser.getMessage();
            if (text.isEmpty())
                return;

            appendDataToDialog(MainWindow::SERVER, text);
        }
    }
}

void MainWindow::connectedUdp()
{
    XMLCommandsParser parser(XMLCommandsParser::CT_INIT, m_clientPort);
    sendUdp(parser.toString());
}

void MainWindow::connectedTcp()
{
    XMLCommandsParser parser(XMLCommandsParser::CT_INIT);
    sendTcp(parser.toString());
}

void MainWindow::sendTcp(const QString &str_)
{
    QString str(str_);
    str.replace("\n", "\t");
    str += '\n';
    socketTcp->write(str.toUtf8());
}

void MainWindow::enableChatWidgets(bool enable)
{
    for (int i = 0; i < ui->dispLayout->count(); ++i)
    {
      ui->dispLayout->itemAt(i)->widget()->setEnabled(enable);
    }

    for (int i = 0; i < ui->msgLayout->count(); ++i)
    {
      ui->msgLayout->itemAt(i)->widget()->setEnabled(enable);
    }
}

void MainWindow::enableTuneUpWidgets(bool enable)
{
    for (int i = 0; i < ui->tuneUpLayout->count(); ++i)
    {
      ui->tuneUpLayout->itemAt(i)->widget()->setEnabled(enable);
    }
}


void MainWindow::sendUdp(const QString &str_)
{
    QString str(str_);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << qint16(0);
    out << str;
    out.device()->seek(qint16(0));
    int sz = data.size();
    out << qint16(data.size() - sizeof(qint16));
    udpOutSocket->writeDatagram(data, QHostAddress(m_host), quint16(m_portHost));
}

void MainWindow::on_clCB_currentIndexChanged(int index)
{
    if (index == UDP)
    {
        ui->clPortL->setEnabled(true);
        ui->clPortLE->setEnabled(true);
        ui->clPortLE->setText(QString::number(m_clientPort));
    }
    else
    {
        ui->clPortL->setEnabled(false);
        ui->clPortLE->setEnabled(false);
        ui->clPortLE->clear();
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    enableTuneUpWidgets(true);
    enableChatWidgets(false);

    ui->actionServerConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
}
