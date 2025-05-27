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
#include <QJsonArray>
#include <QJsonObject>
const QString IPHONE_PATH = "C:/Users/home/Desktop/qq/icons/ipone.png";
const QString GRUPA_PATH = "C:/Users/home/Desktop/qq/icons/grupa.png";
const QString SETING_PATH = "C:/Users/home/Desktop/qq/icons/seting.png";
const QString PROFILE_PATH = "C:/Users/home/Desktop/qq/icons/profile.png";
OKNO::OKNO(Setting* setting, QWidget *parent) : QWidget(parent), m_setting(setting),
    mesage(new QLabel(this)),
    grupa(new QLabel(this)),
    seting(new QLabel(this)),
    profile(new QLabel(this)),
    friendsList(nullptr)
{
    pendingRequestsList = QSharedPointer<QListWidget>(new QListWidget());
    friendIdInput = new QLineEdit(this);
        friendIdInput->setPlaceholderText("Введите Nickname#Tag");
        friendIdInput->setGeometry(500, 150, 200, 30);
        friendIdInput->hide();

        addFriendButton = new QPushButton("Добавить", this);
        addFriendButton->setGeometry(710, 150, 80, 30);
        addFriendButton->hide();
        // Добавляем кнопки
            acceptButton = new QPushButton("Принять", this);
            acceptButton->setGeometry(500, 450, 100, 30);

            rejectButton = new QPushButton("Отклонить", this);
            rejectButton->setGeometry(620, 450, 100, 30);

            // Подключаем сигналы
            connect(acceptButton, &QPushButton::clicked, this, &OKNO::acceptFriendRequest);
            connect(rejectButton, &QPushButton::clicked, this, &OKNO::rejectFriendRequest);

        // Создание списка входящих запросов
           pendingRequestsList.reset(new QListWidget(this));
           pendingRequestsList->setGeometry(500, 200, 400, 200);
           pendingRequestsList->hide();
//    pendingRequestsList = QSharedPointer<QListWidget>();
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
    connect(setting, &Setting::colorsUpdated, this, &OKNO::updateWindowColors);

   }
void OKNO::updateWindowColors(const QColor& background, const QColor& text) {
    this->setStyleSheet(QString("background-color: %1;").arg(background.name()));

    foreach(QWidget* widget, this->findChildren<QWidget*>()) {
        if (qobject_cast<QLabel*>(widget)) {
            widget->setStyleSheet(QString("color: %1;").arg(text.name()));
        }
        // Можно добавить стили для других виджетов
    }
}
void OKNO::showGroupActions() {
    if (!friendsList) {
        friendsList = QSharedPointer<QListWidget>(new QListWidget(this));
        friendsList->setGeometry(1000, 0, 280, 640);
        friendsList->setStyleSheet("background-color: white;");
        friendsList->addItem("Друг 1");
        friendsList->addItem("Друг 2");
        friendsList->addItem("Друг 3");

        friendIdInput->show();
        addFriendButton->show();
        connect(addFriendButton, &QPushButton::clicked, this, [this]{
                sendFriendRequest(friendIdInput->text());
            });
    }
    if (friendsList) {
        friendsList->show();
    }

    if (!pendingRequestsList) {
        pendingRequestsList = QSharedPointer<QListWidget>(new QListWidget(this));
        pendingRequestsList->setGeometry(500, 200, 400, 300);
        pendingRequestsList->setWindowTitle("Добавить друга");
        pendingRequestsList->setStyleSheet("background-color: white;");
    }
    if (pendingRequestsList) {
        pendingRequestsList->show();
    }
}

void OKNO::showSetting(){
    m_setting->show();
}

void OKNO::sendFriendRequest(const QString& friendId) {
    QStringList parts = friendId.split('#');
    if(parts.size() != 2 || parts[0].isEmpty() || parts[1].isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Некорректный формат. Используйте Nickname#Tag");
        return;
    }

    QString friendUsername = parts[0];

    QJsonObject request;
    request["command"] = "SEND_FRIEND_REQUEST";
    request["from"] = m_setting->getCurrentUsername();

    // Отправляем только username, без prefix, так как его генерирует сервер
    request["to"] = friendUsername;

    socketObj->write(QJsonDocument(request).toJson());
}


//Обновление списков
void OKNO::updateFriendsList() {
    if (!friendsList) {
        friendsList = QSharedPointer<QListWidget>(new QListWidget(this));
        friendsList->setGeometry(1000, 0, 280, 640);
        friendsList->setStyleSheet("background-color: white;");
    }
    friendsList->show();
    friendsList->clear();

    for(const auto& friendId : friends.keys()) {
        QListWidgetItem *item = new QListWidgetItem(friendId);
        friendsList->addItem(item);
    }
}

void OKNO::updatePendingRequestsList() {
    pendingRequestsList->clear();
    // Перебираем карту через итератор
    for(auto it = pendingRequests.begin(); it != pendingRequests.end(); ++it) {
        QListWidgetItem *item = new QListWidgetItem(it.value());
        item->setData(Qt::UserRole, it.key()); // Сохраняем ID запроса
        pendingRequestsList->addItem(item);
    }
}

void OKNO::processFriendResponse(const QJsonObject& response) {
    QString requestId = response["request_id"].toString();
    QString friendUsername = response["friend_username"].toString();
    QString friendPrefix = response["friend_prefix"].toString();

    if (response["status"].toString() == "ERROR") {
        QString errorMessage = response["message"].toString();
        QMessageBox::warning(this, "Ошибка", errorMessage);
        return;
    }

    QString action = response["action"].toString();

    if (action == "incoming") {
        // Получили новый запрос
        QString senderUsername = response["sender_username"].toString();
        pendingRequests[requestId] = senderUsername;
        updatePendingRequestsList();
    } else if (action == "accepted") {
        // Запрос принят
        QString friendUsername = response["friend_username"].toString();
        QString friendPrefix = response["friend_prefix"].toString();
        QString friendId = friendUsername + (!friendPrefix.isEmpty() ? "#" + friendPrefix : "");
        friends[friendId] = friendUsername;
        // Добавляем друга в список
        pendingRequests.remove(requestId);
        updatePendingRequestsList();
        updateFriendsList();
    } else if (action == "rejected") {
        // Запрос отклонен
        pendingRequests.remove(requestId);
        updatePendingRequestsList();
    }
}


void OKNO::acceptFriendRequest() {
    QListWidgetItem* selectedItem = pendingRequestsList->currentItem();
    if (!selectedItem) return;

    QString requestId = selectedItem->data(Qt::UserRole).toString();
    QJsonObject request;
    request["command"] = "ACCEPT_FRIEND_REQUEST";
    request["request_id"] = requestId;

    socketObj->write(QJsonDocument(request).toJson());
}

void OKNO::rejectFriendRequest() {
    QListWidgetItem* selectedItem = pendingRequestsList->currentItem();
    if (!selectedItem) return;

    QString requestId = selectedItem->data(Qt::UserRole).toString();
    QJsonObject request;
    request["command"] = "REJECT_FRIEND_REQUEST";
    request["request_id"] = requestId;

    socketObj->write(QJsonDocument(request).toJson());
}

void OKNO::handleIncomingRequest(const QJsonObject& request) {
    QString requestId = request["request_id"].toString();
    QString senderUsername = request["sender_username"].toString();
    QString senderPrefix = request["sender_prefix"].toString();

    // Добавляем запрос в список ожидающих
    pendingRequests[requestId] = senderUsername;
    updatePendingRequestsList();
}
void OKNO::handleFriendsList(const QJsonArray& friends) {
    QMap<QString, QString> friendsMap;
 for(const auto& friendItem : friends) {
 QString friendId = friendItem.toString();
 friendsMap.insert(friendId, friendId);

 }
 updateFriendsList();
}

OKNO::~OKNO() {
    delete socketObj;
}
