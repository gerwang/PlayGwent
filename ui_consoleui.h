/********************************************************************************
** Form generated from reading UI file 'consoleui.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSOLEUI_H
#define UI_CONSOLEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConsoleUI
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ConsoleUI)
    {
        if (ConsoleUI->objectName().isEmpty())
            ConsoleUI->setObjectName(QStringLiteral("ConsoleUI"));
        ConsoleUI->resize(1055, 690);
        centralWidget = new QWidget(ConsoleUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);


        verticalLayout->addLayout(horizontalLayout);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_2->addWidget(label_6);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_2->addWidget(label_7);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_2->addWidget(label_8);

        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_2->addWidget(label_9);

        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_2->addWidget(label_10);


        verticalLayout->addLayout(horizontalLayout_2);

        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        verticalLayout->addWidget(lineEdit_2);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        ConsoleUI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ConsoleUI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1055, 23));
        ConsoleUI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ConsoleUI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ConsoleUI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ConsoleUI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ConsoleUI->setStatusBar(statusBar);

        retranslateUi(ConsoleUI);

        QMetaObject::connectSlotsByName(ConsoleUI);
    } // setupUi

    void retranslateUi(QMainWindow *ConsoleUI)
    {
        ConsoleUI->setWindowTitle(QApplication::translate("ConsoleUI", "ConsoleUI", Q_NULLPTR));
        label->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_2->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_3->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_4->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_5->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ConsoleUI", "PushButton", Q_NULLPTR));
        label_6->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_7->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_8->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_9->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        label_10->setText(QApplication::translate("ConsoleUI", "TextLabel", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ConsoleUI", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConsoleWindow: public Ui_ConsoleUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLEUI_H
