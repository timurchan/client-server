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

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));

    default_connections();

    //ui->lineEdit->setText("127.0.0.1");
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

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    //ui->lineEdit->setText(QString("We'are using ") + arg1 + QString(" connection"));
}

void MainWindow::on_pbConnect_clicked()
{
    //ui->lineEdit->setText(QString("Pushed Connect button"));
    //socket->connectToHost(ui->lineEdit->text(), 4200);
}

void MainWindow::on_actionServerConnect_triggered()
{
    ui->statusBar->showMessage(QString("Trying to connect to the server..."));



    //socket->connectToHost(QString("1.1.1.1"), 4200);
    socket->connectToHost(m_host, m_port);
    ui->statusBar->showMessage(QString("server ip: ") + socket->peerName() +
                               QString(", port: ") + QString::number((int)socket->peerPort()));
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

// This function gets called whenever the chat server has sent us some text:
void MainWindow::readyRead()
{
    // We'll loop over every (complete) line of text that the server has sent us:
    while(socket->canReadLine())
    {
        // Here's the line the of text the server sent us (we use UTF-8 so
        // that non-English speakers can chat in their native language)
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        // These two regular expressions describe the kinds of messages
        // the server can send us:

        //  Normal messges look like this: "username:The message"
        //QRegExp messageRegex("^([^:]+):(.*)$");

        // Any message that starts with "/users:" is the server sending us a
        // list of users so we can show that list in our GUI:
        QRegExp usersRegex("^/users:(.*)$");

        // Is this a users message:
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
        // Is this a normal chat message:
//        else if(messageRegex.indexIn(line) != -1)
//        {
//            // If so, append this message to our chat box:
//            QString user = messageRegex.cap(1);
//            QString message = messageRegex.cap(2);

//            roomTextEdit->append("<b>" + user + "</b>: " + message);
//        }
    }
}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void MainWindow::connected()
{
    //socket->write(QString("/me:" + userLineEdit->text() + "\n").toUtf8());
    socket->write(QString("initString\n").toUtf8());
}

void MainWindow::on_applyPB_clicked()
{
    // have to extract data from controls (host, port, protocol type)
    tuneUpWidget->hide();
    m_host = tuneUpUI->hostLE->text();
    bool ok = false;
    const int port = tuneUpUI->portLE->text().toInt(&ok);
    m_port = ok ? port : 4200;
//    if (ok)
//    {
//        m_port = port;
//    }
//    else
//    {
//        m_port = 4200;
//    }
    const int idx = tuneUpUI->protocolCB->currentIndex();
    m_protocol_type = tuneUpUI->protocolCB->itemText(idx);
}

void MainWindow::default_connections()
{
    m_host = QString("127.0.0.1");
    m_port = 4200;
    m_protocol_type = "TCP";
    tuneUpUI->hostLE->setText(m_host);
    tuneUpUI->portLE->setText(QString::number(m_port));
    tuneUpUI->protocolCB->setCurrentIndex(0);
}
