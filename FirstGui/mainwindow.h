#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QVector>

namespace Ui {
    class MainWindow;
    class TuneUpForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum Protocol
    {
        TCP = 0,
        UDP = 1
    };

    enum User
    {
        SERVER = 0,
        CLIENT = 1
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onClickApplyTuneButton();
    void on_actionServerConnect_triggered();
    void on_actionClientTune_triggered();
    void on_actionExit_triggered();
    void on_messageLineEdit_returnPressed();

    void readyReadTcp();
    void readyReadUdp();
    void connectedTcp();
    void connectedUdp();

private:
    Ui::MainWindow *ui;
    Ui::TuneUpForm *tuneUpUi;
    QWidget *tuneUpWidget;

    QString m_host;
    Protocol m_protocol_type;
    int m_port;
    int m_defaultPort;
    bool m_isBindUdpInSocket;

    QTcpSocket *socketTcp;
    QUdpSocket *socketUdp;
    QUdpSocket *udpOutSocket;

    QVector<QListWidgetItem*> userIcons;
private: // service function
    void init_server_connection();
    void appendDataToDialog(MainWindow::User user, const QString &str);
    void showUsers(const QStringList& users);
    void sendUdp(const QString& str);
    void sendTcp(const QString& str);

private: // constants
    static const int DEFAULT_PORT;
    static const int DEFAULT_PORT_CLIENT;
    static const QString DEFAULT_HOST;
    static const QString SERVER_COLOR;
    static const QString CLIENT_COLOR;

};

#endif // MAINWINDOW_H
