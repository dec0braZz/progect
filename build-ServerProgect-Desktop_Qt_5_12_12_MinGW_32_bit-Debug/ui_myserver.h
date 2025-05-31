/********************************************************************************
** Form generated from reading UI file 'myserver.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYSERVER_H
#define UI_MYSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyServer
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MyServer)
    {
        if (MyServer->objectName().isEmpty())
            MyServer->setObjectName(QString::fromUtf8("MyServer"));
        MyServer->resize(800, 600);
        centralwidget = new QWidget(MyServer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MyServer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MyServer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MyServer->setMenuBar(menubar);
        statusbar = new QStatusBar(MyServer);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MyServer->setStatusBar(statusbar);

        retranslateUi(MyServer);

        QMetaObject::connectSlotsByName(MyServer);
    } // setupUi

    void retranslateUi(QMainWindow *MyServer)
    {
        MyServer->setWindowTitle(QApplication::translate("MyServer", "MyServer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyServer: public Ui_MyServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYSERVER_H
