/********************************************************************************
** Form generated from reading UI file 'tune_connection.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TUNE_CONNECTION_H
#define UI_TUNE_CONNECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *protocolL;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QLabel *hostLabel;
    QComboBox *protocolCB;
    QLineEdit *hostLE;
    QLabel *portLabel;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *portLE;
    QPushButton *applyPB;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(283, 180);
        gridLayoutWidget = new QWidget(Form);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 10, 241, 161));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        protocolL = new QLabel(gridLayoutWidget);
        protocolL->setObjectName(QStringLiteral("protocolL"));

        gridLayout->addWidget(protocolL, 3, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 4, 0, 1, 3);

        hostLabel = new QLabel(gridLayoutWidget);
        hostLabel->setObjectName(QStringLiteral("hostLabel"));

        gridLayout->addWidget(hostLabel, 0, 0, 1, 1);

        protocolCB = new QComboBox(gridLayoutWidget);
        protocolCB->setObjectName(QStringLiteral("protocolCB"));

        gridLayout->addWidget(protocolCB, 3, 1, 1, 1);

        hostLE = new QLineEdit(gridLayoutWidget);
        hostLE->setObjectName(QStringLiteral("hostLE"));

        gridLayout->addWidget(hostLE, 0, 1, 1, 2);

        portLabel = new QLabel(gridLayoutWidget);
        portLabel->setObjectName(QStringLiteral("portLabel"));

        gridLayout->addWidget(portLabel, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 2, 1, 1);

        portLE = new QLineEdit(gridLayoutWidget);
        portLE->setObjectName(QStringLiteral("portLE"));

        gridLayout->addWidget(portLE, 2, 1, 1, 1);

        applyPB = new QPushButton(gridLayoutWidget);
        applyPB->setObjectName(QStringLiteral("applyPB"));

        gridLayout->addWidget(applyPB, 5, 2, 1, 1);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        protocolL->setText(QApplication::translate("Form", "protocol", 0));
        hostLabel->setText(QApplication::translate("Form", "host:", 0));
        protocolCB->clear();
        protocolCB->insertItems(0, QStringList()
         << QApplication::translate("Form", "TCP", 0)
         << QApplication::translate("Form", "UDP", 0)
        );
        portLabel->setText(QApplication::translate("Form", "port:", 0));
        applyPB->setText(QApplication::translate("Form", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TUNE_CONNECTION_H
