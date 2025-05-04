#include "okno.h"
#include"ColorPalette.h"
#include <QEvent>
#include <QMovie>
#include <QTimer>
#include <QWidget>
#include <QPropertyAnimation>
#include <QDebug>
#include "setting.h"
#include <QMouseEvent>
#include "socket.h"
const QString IPHONE_PATH = "C:/Users/home/Desktop/qq/icons/ipone.gif";
const QString GRUPA_PATH = "C:/Users/home/Desktop/qq/icons/grupa.gif";
const QString SETING_PATH = "C:/Users/home/Desktop/qq/icons/seting.gif";
const QString PROFILE_PATH = "C:/Users/home/Desktop/qq/icons/profile.gif";
OKNO::OKNO(Setting* setting, QWidget *parent) : QWidget(parent), m_setting(setting),
    mesage(new QLabel(this)),
    grupa(new QLabel(this)),
    seting(new QLabel(this)),
    profile(new QLabel(this)) {
    socketObj = new Socket(this);  // создаём объект Socket
    setWindowTitle("KLOZ");
    setStyleSheet("background-color: grey;");
    resize(1280, 640);
    QMenuBar* menuBar = new QMenuBar(this);
    QToolBar* toolBar = new QToolBar(this);
    toolBar->setOrientation(Qt::Vertical);  // Вертикальная ориентация

    toolBar->setMovable(true);
    toolBar->setFloatable(true);
    toolBar->setGeometry(0, 0, 150, height());  // например, узкая и высокая панель
    toolBar->layout()->setSpacing(70);
    toolBar->setIconSize(QSize(64, 64));  // Устанавливаем размер иконок у тулбара
   // toolBar->setContentsMargins(10, 10, 10, 10);
    toolBar->setStyleSheet(R"(
        QToolButton {

            min-height: 40px;
            min-width: 100px;
            font-size: 14pt;
            padding: 10px;


        }
    )");


    // Создаем действия
    QAction* messageAction = new QAction(QIcon(IPHONE_PATH), " Быстрый набор ", this);
    // Не работает messageAction->setIconSize(QSize(64, 64));  // задать размер иконки
    QAction* groupAction = new QAction(QIcon(GRUPA_PATH), " Группы ", this);
    QAction* settingsAction = new QAction(QIcon(SETING_PATH), " Настройки ", this);
    QAction* profileAction = new QAction(QIcon(PROFILE_PATH), " Профиль ", this);

    // Добавляем действия в тулбар
    toolBar->addAction(messageAction);
    toolBar->addAction(groupAction);
    toolBar->addAction(settingsAction);
    toolBar->addAction(profileAction);

    // Подключаем сигналы
    connect(groupAction, &QAction::triggered, this, &OKNO::showGroupActions);
    connect(settingsAction, &QAction::triggered, this, &OKNO::showSetting);
}

void OKNO::showGroupActions() {
    // Сначала проверяем, существуют ли уже списки
    if (!friendsList) {
        friendsList = new QListWidget(this);
        friendsList->setGeometry(1000, 0, 280, 640);
        friendsList->setStyleSheet("background-color: white;");
        friendsList->addItem("Друг 1");
        friendsList->addItem("Друг 2");
        friendsList->addItem("Друг 3");
    }
    friendsList->show();

    if (!addFriendList) {
        addFriendList = new QListWidget(this);
        addFriendList->setGeometry(500, 200, 400, 300);
        addFriendList->setWindowTitle("Добавить друга");
    }
    addFriendList->show();
}
void OKNO::showSetting(){
    m_setting->show();
}


OKNO::~OKNO() {
    delete friendsList;
    delete addFriendList;
    delete socketObj;
}
