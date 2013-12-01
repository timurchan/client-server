#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_tuneUpPanel.h"

const QString MainWindow::DEFAULT_HOST = QString("127.0.0.1");
const int     MainWindow::DEFAULT_PORT = 4200;
const QString MainWindow::SERVER_COLOR = QString("red");
const QString MainWindow::CLIENT_COLOR = QString("blue");


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tuneUpUi(new Ui::TuneUpForm),
    tuneUpWidget(new QWidget()),
    m_port(DEFAULT_PORT),
    m_host(DEFAULT_HOST),
    m_protocol_type(MainWindow::UDP)
{
    ui->setupUi(this);
    tuneUpUi->setupUi(tuneUpWidget);

    ui->dialogW->setReadOnly(true);
    init_server_connection();


    connect(tuneUpUi->applyPushButton, SIGNAL(clicked()), this, SLOT(onClickApplyTuneButton()));

//    ui->lineEdit->setText("127.0.0.1");
/*
    btn = new QPushButton("Apply", this);
    btn->show();
    connect(btn, SIGNAL(clicked()), this, SLOT(onBtnClicked()));

    edit = new QLineEdit(this);
    edit->move(50, 50);
    edit->show();

*/

    //QStringList lst;
//    lwg = new QListWidget(this);
//    QListWidgetItem* pitem = 0;

    //lwg->setIconSize(QSize(48, 48));
//    lst << "Linux" << "Windows" << "MacOS" << "OS2";
//    foreach(QString str, lst) {
//        pitem = new QListWidgetItem(str, lwg);
//        //pitem->setIcon(QPixmap(str + ".jpg"));
//    }
//    lwg->resize(200, 200);
//    lwg->move(50,50);
//    lwg->show();

//    ui->stoping
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tuneUpUi;
    delete tuneUpWidget;
}

/*
void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_comboBox_activated(const QString &arg1)
{

}

void MainWindow::onBtnClicked()
{
    static int counter = 0;

    edit->setText("123");
    lwg->addItem(QString::number(counter++));
}
*/
void MainWindow::on_actionServerConnect_triggered()
{
    ui->statusBar->showMessage(QString("Trying to connect to the server..."));

    if(m_protocol_type == UDP)
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

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    //ui->lineEdit->setText(QString("We'are using ") + arg1 + QString(" connection"));
}

void MainWindow::on_pbConnect_clicked()
{
    //ui->lineEdit->setText(QString("Pushed Connect button"));
    //socket->connectToHost(ui->lineEdit->text(), 4200);
}

void MainWindow::onClickApplyTuneButton()
{
    // TODO:
    bool ok = false;
    const int port = tuneUpUi->portLineEdit->text().toInt(&ok);
    m_port = ok ? port : MainWindow::DEFAULT_PORT;

    m_host = tuneUpUi->hostLineEdit->text();

    const int idxCB = tuneUpUi->protocolComboBox->currentIndex();
    m_protocol_type = (idxCB == 0) ? MainWindow::TCP : MainWindow::UDP;

    tuneUpWidget->setVisible(false);
}


void MainWindow::on_actionServerTune_triggered()
{
    ui->statusBar->showMessage(QString("Tune up server connection"));
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
void MainWindow::on_actionClientTune_triggered()
{
    ui->statusBar->showMessage(QString("Tune up client connection"));
    if (tuneUpWidget == NULL)
    {
        tuneUpWidget = new QWidget();
        tuneUpUi->setupUi(tuneUpWidget);
        init_server_connection();
    }
    tuneUpWidget->setVisible(true);
}

void MainWindow::on_actionExit_triggered()
{
    ui->statusBar->showMessage(QString("Close connection"));
    this->close();
}



void MainWindow::init_server_connection()
{
    tuneUpUi->hostLineEdit->setText(m_host);
    tuneUpUi->portLineEdit->setText(QString::number(m_port));
    tuneUpUi->protocolComboBox->setCurrentIndex(m_protocol_type);
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
    const QString text = ui->messageLineEdit->text();
    if (text.isEmpty())
        return;

    appendDataToDialog(MainWindow::CLIENT, text);

    /// TODO: write to socket
    //socket->write(text.toUtf8());



    //socket->flush()
    ui->messageLineEdit->clear();

}

//void MainWindow::on_messageLineEdit_returnPressed()
//{

//}



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

    udpOutSocket->writeDatagram("Servers, where are you?\n", QHostAddress("127.0.0.1"), quint16(m_port) );
}

void MainWindow::connectedTcp()
{
    //socket->write(QString("/me:" + userLineEdit->text() + "\n").toUtf8());
    socketTcp->write(QString("initString\n").toUtf8());
}
