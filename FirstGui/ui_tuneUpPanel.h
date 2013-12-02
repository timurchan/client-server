/********************************************************************************
** Form generated from reading UI file 'tuneUpPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TUNEUPPANEL_H
#define UI_TUNEUPPANEL_H

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

class Ui_TuneUpForm
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *protocolLabel;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QLabel *hostLabel;
    QComboBox *protocolComboBox;
    QLineEdit *hostLineEdit;
    QLabel *portLabel;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *portLineEdit;
    QPushButton *applyPushButton;

    void setupUi(QWidget *TuneUpForm)
    {
        if (TuneUpForm->objectName().isEmpty())
            TuneUpForm->setObjectName(QStringLiteral("TuneUpForm"));
        TuneUpForm->resize(283, 180);
        gridLayoutWidget = new QWidget(TuneUpForm);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 10, 241, 161));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        protocolLabel = new QLabel(gridLayoutWidget);
        protocolLabel->setObjectName(QStringLiteral("protocolLabel"));

        gridLayout->addWidget(protocolLabel, 3, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 4, 0, 1, 3);

        hostLabel = new QLabel(gridLayoutWidget);
        hostLabel->setObjectName(QStringLiteral("hostLabel"));

        gridLayout->addWidget(hostLabel, 0, 0, 1, 1);

        protocolComboBox = new QComboBox(gridLayoutWidget);
        protocolComboBox->setObjectName(QStringLiteral("protocolComboBox"));

        gridLayout->addWidget(protocolComboBox, 3, 1, 1, 1);

        hostLineEdit = new QLineEdit(gridLayoutWidget);
        hostLineEdit->setObjectName(QStringLiteral("hostLineEdit"));

        gridLayout->addWidget(hostLineEdit, 0, 1, 1, 2);

        portLabel = new QLabel(gridLayoutWidget);
        portLabel->setObjectName(QStringLiteral("portLabel"));

        gridLayout->addWidget(portLabel, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 2, 1, 1);

        portLineEdit = new QLineEdit(gridLayoutWidget);
        portLineEdit->setObjectName(QStringLiteral("portLineEdit"));

        gridLayout->addWidget(portLineEdit, 2, 1, 1, 1);

        applyPushButton = new QPushButton(gridLayoutWidget);
        applyPushButton->setObjectName(QStringLiteral("applyPushButton"));

        gridLayout->addWidget(applyPushButton, 5, 2, 1, 1);


        retranslateUi(TuneUpForm);

        QMetaObject::connectSlotsByName(TuneUpForm);
    } // setupUi

    void retranslateUi(QWidget *TuneUpForm)
    {
        TuneUpForm->setWindowTitle(QApplication::translate("TuneUpForm", "Form", 0));
        protocolLabel->setText(QApplication::translate("TuneUpForm", "protocol", 0));
        hostLabel->setText(QApplication::translate("TuneUpForm", "host:", 0));
        protocolComboBox->clear();
        protocolComboBox->insertItems(0, QStringList()
         << QApplication::translate("TuneUpForm", "TCP", 0)
         << QApplication::translate("TuneUpForm", "UDP", 0)
        );
        portLabel->setText(QApplication::translate("TuneUpForm", "port:", 0));
        applyPushButton->setText(QApplication::translate("TuneUpForm", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class TuneUpForm: public Ui_TuneUpForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TUNEUPPANEL_H
