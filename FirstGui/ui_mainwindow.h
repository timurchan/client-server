/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionServerConnect;
    QAction *actionClientTune;
    QAction *actionExit;
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *clientsL;
    QPlainTextEdit *plainTextEdit;
    QLabel *messageLbl;
    QListView *clientsView;
    QListWidget *clientsWidget;
    QLineEdit *leMessage;
    QPushButton *pbProcessMessage;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(488, 388);
        actionServerConnect = new QAction(MainWindow);
        actionServerConnect->setObjectName(QStringLiteral("actionServerConnect"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/icons/server.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionServerConnect->setIcon(icon);
        actionClientTune = new QAction(MainWindow);
        actionClientTune->setObjectName(QStringLiteral("actionClientTune"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/icons/setting_tools.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClientTune->setIcon(icon1);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/prefix1/icons/door_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 11, 471, 311));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        clientsL = new QLabel(gridLayoutWidget);
        clientsL->setObjectName(QStringLiteral("clientsL"));

        gridLayout->addWidget(clientsL, 0, 2, 1, 1);

        plainTextEdit = new QPlainTextEdit(gridLayoutWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 2, 0, 1, 2);

        messageLbl = new QLabel(gridLayoutWidget);
        messageLbl->setObjectName(QStringLiteral("messageLbl"));

        gridLayout->addWidget(messageLbl, 5, 0, 1, 1);

        clientsView = new QListView(gridLayoutWidget);
        clientsView->setObjectName(QStringLiteral("clientsView"));

        gridLayout->addWidget(clientsView, 3, 2, 1, 1);

        clientsWidget = new QListWidget(gridLayoutWidget);
        clientsWidget->setObjectName(QStringLiteral("clientsWidget"));

        gridLayout->addWidget(clientsWidget, 2, 2, 1, 1);

        leMessage = new QLineEdit(gridLayoutWidget);
        leMessage->setObjectName(QStringLiteral("leMessage"));

        gridLayout->addWidget(leMessage, 5, 1, 1, 1);

        pbProcessMessage = new QPushButton(gridLayoutWidget);
        pbProcessMessage->setObjectName(QStringLiteral("pbProcessMessage"));

        gridLayout->addWidget(pbProcessMessage, 5, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionServerConnect);
        mainToolBar->addAction(actionClientTune);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Diakont client", 0));
        actionServerConnect->setText(QApplication::translate("MainWindow", "ServerConnect", 0));
#ifndef QT_NO_TOOLTIP
        actionServerConnect->setToolTip(QApplication::translate("MainWindow", "connect", 0));
#endif // QT_NO_TOOLTIP
        actionClientTune->setText(QApplication::translate("MainWindow", "ClientTune", 0));
#ifndef QT_NO_TOOLTIP
        actionClientTune->setToolTip(QApplication::translate("MainWindow", "client tune", 0));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("MainWindow", "exit", 0));
#endif // QT_NO_TOOLTIP
        clientsL->setText(QApplication::translate("MainWindow", "Clients:", 0));
        messageLbl->setText(QApplication::translate("MainWindow", "Message:", 0));
        pbProcessMessage->setText(QApplication::translate("MainWindow", "Text to server for frocessing", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
