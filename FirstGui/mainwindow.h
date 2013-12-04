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
    void getNewSettings();
    void on_actionServerConnect_triggered();
    void on_actionExit_triggered();
    void on_messageLineEdit_returnPressed();
    void on_clCB_currentIndexChanged(int index);
    void on_actionDisconnect_triggered();

    void readyReadTcp();
    void readyReadUdp();
    void connectedTcp();
    void connectedUdp();

protected slots:
    virtual void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;

    QString m_host;
    Protocol m_protocol_type;
    int m_portHost;
    int m_clientPort;
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

    void enableChatWidgets(bool show);
    void enableTuneUpWidgets(bool show);
    void clearUserIcons();

private: // constants
    static const int DEFAULT_PORT;
    static const int DEFAULT_PORT_CLIENT;
    static const QString DEFAULT_HOST;
    static const QString SERVER_COLOR;
    static const QString CLIENT_COLOR;

};

#endif // MAINWINDOW_H
