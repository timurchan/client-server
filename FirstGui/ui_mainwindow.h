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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
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
    QLineEdit *messageLineEdit;
    QLabel *messageLabel;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QListWidget *clientsWidget;
    QLabel *dialogLabel;
    QLabel *clientsLabel;
    QTextEdit *dialogW;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(391, 410);
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
        gridLayoutWidget->setGeometry(QRect(10, 291, 371, 61));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        messageLineEdit = new QLineEdit(gridLayoutWidget);
        messageLineEdit->setObjectName(QStringLiteral("messageLineEdit"));

        gridLayout->addWidget(messageLineEdit, 0, 1, 1, 1);

        messageLabel = new QLabel(gridLayoutWidget);
        messageLabel->setObjectName(QStringLiteral("messageLabel"));

        gridLayout->addWidget(messageLabel, 0, 0, 1, 1);

        gridLayoutWidget_2 = new QWidget(centralWidget);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 10, 371, 271));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        clientsWidget = new QListWidget(gridLayoutWidget_2);
        clientsWidget->setObjectName(QStringLiteral("clientsWidget"));

        gridLayout_2->addWidget(clientsWidget, 1, 1, 1, 1);

        dialogLabel = new QLabel(gridLayoutWidget_2);
        dialogLabel->setObjectName(QStringLiteral("dialogLabel"));

        gridLayout_2->addWidget(dialogLabel, 0, 0, 1, 1);

        clientsLabel = new QLabel(gridLayoutWidget_2);
        clientsLabel->setObjectName(QStringLiteral("clientsLabel"));

        gridLayout_2->addWidget(clientsLabel, 0, 1, 1, 1);

        dialogW = new QTextEdit(gridLayoutWidget_2);
        dialogW->setObjectName(QStringLiteral("dialogW"));

        gridLayout_2->addWidget(dialogW, 1, 0, 1, 1);

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
        messageLabel->setText(QApplication::translate("MainWindow", "Message:", 0));
        dialogLabel->setText(QApplication::translate("MainWindow", "Dialog", 0));
        clientsLabel->setText(QApplication::translate("MainWindow", "Clients:", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
