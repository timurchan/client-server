#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_tune_connection.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    tuneUpWidget(new QWidget()),
    ui(new Ui::MainWindow),
    tuneUpUI(new Ui::Form)
{
    ui->setupUi(this);
    tuneUpUI->setupUi(tuneUpWidget);

    m_defaultPort = 5300;
    default_connections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

    //ui->lineEdit->setText(QString("We'are using ") + arg1 + QString(" connection"));
}

void MainWindow::on_pbConnect_clicked()
{

}

void MainWindow::on_actionServerConnect_triggered()
{
    ui->statusBar->showMessage(QString("Trying to connect to the server..."));

    if(m_protocolType == UDP)
    {
        socketUdp = new QUdpSocket(this);
        bool isBind = socketUdp->bind(QHostAddress::LocalHost, 7755); // listen from server
        if(isBind)
        {
            connect(socketUdp, SIGNAL(readyRead()), this, SLOT(readyReadUdp()));

            udpOutSocket = new QUdpSocket(this);
            connectedUdp();

            ui->statusBar->showMessage(QString("server ip: ") + m_host +
                                       QString(", port: ") + QString::number((int)m_port));
        }
        else
        {
            ui->statusBar->showMessage("Bind local port for listen server fail.");
        }

    }
    else // TCP
    {
        socketTcp = new QTcpSocket(this);
        connect(socketTcp, SIGNAL(readyRead()), this, SLOT(readyReadTcp()));
        connect(socketTcp, SIGNAL(connected()), this, SLOT(connectedTcp()));
        socketTcp->connectToHost(m_host, m_port);

        ui->statusBar->showMessage(QString("server ip: ") + m_host +
                                   QString(", port: ") + QString::number((int)m_port));
    }




}

//void MainWindow::on_actionServerTune_triggered()
//{
//    ui->statusBar->showMessage(QString("Tune up server connection"));
//}

void MainWindow::on_actionClientTune_triggered()
{
    ui->statusBar->showMessage(QString("Tune up client connection"));

//    if (&tuneUpWidget == NULL)
//        tuneUpWidget = new QWidget();
    if (tuneUpWidget == NULL)
    {
        tuneUpWidget = new QWidget();
        tuneUpUI->setupUi(tuneUpWidget);
    }


    tuneUpWidget->show();
    connect(tuneUpUI->applyPB, SIGNAL(clicked()), this, SLOT(on_applyPB_clicked()));
}

void MainWindow::on_actionExit_triggered()
{
    ui->statusBar->showMessage(QString("Close connection"));
    this->close();
}

void MainWindow::readyReadUdp()
{
    //QUdpSocket* udpSocket = dynamic_cast<QUdpSocket*>(socket);
    while (socketUdp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketUdp->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socketUdp->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        qDebug() << "sender ip : " << sender.toString()
                 << ", port : " << QString::number(senderPort);
    }
}

void MainWindow::readyReadTcp()
{
    // We'll loop over every (complete) line of text that the server has sent us:
    while(socketTcp->canReadLine())
    {
        // Here's the line the of text the server sent us (we use UTF-8 so
        // that non-English speakers can chat in their native language)
        QString line = QString::fromUtf8(socketTcp->readLine()).trimmed();

        // These two regular expressions describe the kinds of messages
        // the server can send us:

        //  Normal messges look like this: "username:The message"
        //QRegExp messageRegex("^([^:]+):(.*)$");

        // Any message that starts with "/users:" is the server sending us a
        // list of users so we can show that list in our GUI:
        QRegExp usersRegex("^/users:(.*)$");

        // users message:
        if(usersRegex.indexIn(line) != -1)
        {
            //QListWidgetItem* pitem = 0;
            //lwg->clear();
            //lwg->setIconSize(QSize(24, 24));
            ui->clientsWidget->clear();
            ui->clientsWidget->setIconSize(QSize(24, 24));

            // If so, udpate our users list on the right:
            QStringList users = usersRegex.cap(1).split(",");
            //userListWidget->clear();
            foreach(QString user, users)
            {
                new QListWidgetItem(QPixmap("user.png"), user, ui->clientsWidget);
                    //pitem = new QListWidgetItem(user, lwg);
                    //pitem->setIcon(QPixmap(str + ".jpg"));


            //new QListWidgetItem(QPixmap(":/user.png"), user, userListWidget);
            }
        }
        // result after process client`s message
        //else if(messageRegex.indexIn(line) != -1)
        else
        {
            ui->leMessage->setText(line);
        }
    }
}

void MainWindow::connectedUdp()
{
    QString str = "initString\n";
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << qint16(0);
    out << str;
    out.device()->seek(qint16(0));
    int sz = data.size();
    out << qint16(data.size() - sizeof(qint16));
    //udpOutSocket->writeDatagram(data, QHostAddress::LocalHost, 4300);

    udpOutSocket->writeDatagram("Servers, where are you?\n", QHostAddress("127.0.0.1"), quint16(m_defaultPort) );
}

void MainWindow::connectedTcp()
{
    //socket->write(QString("/me:" + userLineEdit->text() + "\n").toUtf8());
    socketTcp->write(QString("initString\n").toUtf8());
}

void MainWindow::on_applyPB_clicked()
{
    // have to extract data from controls (host, port, protocol type)
    tuneUpWidget->hide();
    m_host = tuneUpUI->hostLE->text();
    bool ok = false;
    const int port = tuneUpUI->portLE->text().toInt(&ok);
    m_port = ok ? port : m_defaultPort;

    const int idx = tuneUpUI->protocolCB->currentIndex();
    //m_protocol_type = tuneUpUI->protocolCB->itemText(idx);
    if(tuneUpUI->protocolCB->itemText(idx) == "UDP") {
        m_protocolType = UDP;
    } else { // TCP
        m_protocolType = TCP;
    }
}

void MainWindow::default_connections()
{
    m_host = QString("127.0.0.1");
    m_port = m_defaultPort;
    //m_protocolType = TCP;
    m_protocolType = UDP;
    tuneUpUI->hostLE->setText(m_host);
    tuneUpUI->portLE->setText(QString::number(m_port));
    tuneUpUI->protocolCB->setCurrentIndex(0);
}

void MainWindow::on_pbProcessMessage_clicked()
{
    socketTcp->write(QString("/message:" + ui->leMessage->text() + "\n").toUtf8());
}
