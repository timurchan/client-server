#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QTcpSocket>
#include <QUdpSocket>

namespace Ui {
    class MainWindow;
    class Form;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum ProtocolType {
       UDP,
       TCP
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /*
    void on_pushButton_clicked();
    void on_comboBox_activated(const QString &arg1);
    void onBtnClicked();
    */

    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_pbConnect_clicked();

    void on_actionServerConnect_triggered();

    //void on_actionServerTune_triggered();

    void on_actionClientTune_triggered();

    void on_actionExit_triggered();

    void readyReadTcp();
    void readyReadUdp();
    void connectedTcp();
    void connectedUdp();

    void on_applyPB_clicked();

    void on_pbProcessMessage_clicked();

private: // service functions
    void default_connections();

private:
    QWidget *tuneUpWidget;
    Ui::MainWindow *ui;
    Ui::Form *tuneUpUI;



    QTcpSocket *socketTcp;
    QUdpSocket *socketUdp;
    //QAbstractSocket *socket;
    QUdpSocket *udpOutSocket;

    QString m_host;
    int m_port;
    int m_defaultPort;
    //QString m_protocol_type;
    ProtocolType m_protocolType;

    /*
    QPushButton *btn ;
    QLineEdit *edit;
    */
    QListWidget* lwg;

};

#endif // MAINWINDOW_H
