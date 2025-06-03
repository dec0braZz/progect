#include "okno.h"
#include "chatwindow.h"
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
OKNO::OKNO(Setting* setting, QSharedPointer<QTcpSocket> socket, QWidget *parent)
    : QWidget(parent),
      m_setting(setting),
      friendsList(nullptr),
      isInCall(false)
{
    mesage = new QLabel(this);
    grupa = new QLabel(this);
    seting = new QLabel(this);
    profile = new QLabel(this);
    voiceSocket = new QUdpSocket(this);
    isInCall = false;
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

            // Подключим сигналы от сокета
            connect(tcpSocket.data(), &QTcpSocket::readyRead, this, [this]() {
                    QByteArray data = tcpSocket->readAll();
                    QJsonDocument doc = QJsonDocument::fromJson(data);
                    if (!doc.isNull() && doc.isObject()) {
                        QJsonObject json = doc.object();
                        if (json["command"].toString() == "voice_call") {
                            handleIncomingVoiceCall(json);
                        }
                    }
                });

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
    friendsList->clear();
    for(const auto& friendId : friends.keys()) {
        QListWidgetItem *item = new QListWidgetItem(friendsList.get());
        auto *friendWidget = new FriendWidget(friendId);

        // Подключаем сигнал кнопки к методу открытия чата
        connect(friendWidget, &FriendWidget::chatRequested, this, &OKNO::openChatWindow);
        connect(friendWidget, &FriendWidget::voiceCallRequested, this, [this](const QString &friendName) {
                    sendVoiceCallRequest(friendName);
                });
        item->setSizeHint(friendWidget->sizeHint());
        friendsList->addItem(item);
        friendsList->setItemWidget(item, friendWidget);
    }
    friendsList->show();
}
void OKNO::sendVoiceCallRequest(const QString &friendId) {
    // Убираем проверку на разделение по '#', так как friendId может быть просто username
    QJsonObject request;
    request["command"] = "VOICE_CALL_REQUEST";
    request["from"] = m_setting->getCurrentUsername();
    request["to"] = friendId;  // Отправляем весь friendId как есть

    socketObj->write(QJsonDocument(request).toJson());
    QMessageBox::information(this, "Запрос отправлен", "Запрос на голосовой вызов отправлен пользователю " + friendId);
}
void OKNO::openChatWindow(const QString &friendName) {
    // Создаёте или показываете окно чата с другом friendName
    // Например, создаёте новое окно или виджет диалога
    ChatWindow *chat = new ChatWindow(friendName, this);
    chat->show();
}

// Обработка ответа на голосовой вызов
void OKNO::processVoiceCallResponse(const QJsonObject& response) {
    QString from = response["from"].toString();
    QString result = response["response"].toString();

    if(result == "accept") {
        QMessageBox::information(this, "Вызов принят",
            QString("Пользователь %1 принял ваш вызов. Начинаем голосовой чат.").arg(from));
        // Здесь можно открыть окно голосового чата
    } else {
        QMessageBox::information(this, "Вызов отклонен",
            QString("Пользователь %1 отклонил ваш вызов.").arg(from));
    }
}

// Обработка входящего вызова
void OKNO::processIncomingCall(const QJsonObject& notification) {
    QString from = notification["from"].toString();
    QString fromPrefix = notification["from_prefix"].toString();
    QString fullName = from + "#" + fromPrefix;

    QMessageBox callDialog(this);
    callDialog.setWindowTitle("Входящий вызов");
    callDialog.setText(QString("Входящий голосовой вызов от %1").arg(fullName));

    QPushButton *acceptButton = callDialog.addButton("Принять", QMessageBox::AcceptRole);
    QPushButton *rejectButton = callDialog.addButton("Отклонить", QMessageBox::RejectRole);

    callDialog.exec();

    QJsonObject response;
    response["command"] = "VOICE_CALL_RESPONSE";
    response["to"] = from;
    response["from"] = m_setting->getCurrentUsername();

    if(callDialog.clickedButton() == acceptButton) {
        response["response"] = "accept";
        // Здесь можно открыть окно голосового чата
    } else {
        response["response"] = "reject";
    }

    socketObj->write(QJsonDocument(response).toJson());
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
void OKNO::handleIncomingVoiceCall(const QJsonObject& callData) {
    QString command = callData["command"].toString();
    QString action = callData["action"].toString();

    if (action == "incoming") {
        if(isInCall) {
            QJsonObject response;
            response["command"] = "VOICE_CALL_RESPONSE";
            response["to"] = callData["from"].toString();
            response["from"] = m_setting->getCurrentUsername();
            response["response"] = "reject";
            response["reason"] = "User is busy";

            socketObj->write(QJsonDocument(response).toJson());
            return;
        }

        QString from = callData["from"].toString();
        QString fromPrefix = callData["from_prefix"].toString();
        int callId = callData["call_id"].toInt();

        QMessageBox callDialog(this);
        callDialog.setWindowTitle("Входящий вызов");
        callDialog.setText(QString("Входящий вызов от %1#%2").arg(from).arg(fromPrefix));

        QPushButton *acceptButton = callDialog.addButton("Принять", QMessageBox::AcceptRole);
        QPushButton *rejectButton = callDialog.addButton("Отклонить", QMessageBox::RejectRole);

        callDialog.exec();

        QJsonObject response;
        response["command"] = "VOICE_CALL_RESPONSE";
        response["to"] = from;
        response["from"] = m_setting->getCurrentUsername();
        response["call_id"] = callId;

        if(callDialog.clickedButton() == acceptButton) {
            response["response"] = "accept";
            currentCallPeer = from;
            startVoiceChat(); // Окно открывается сразу у принимающего
        } else {
            response["response"] = "reject";
        }

        socketObj->write(QJsonDocument(response).toJson());
    }
    else if (action == "response") {
        QString from = callData["from"].toString();
        QString result = callData["response"].toString();

        if(result == "accept") {
            QMessageBox::information(this, "Вызов принят",
                QString("Пользователь %1 принял ваш вызов. Начинаем голосовой чат.").arg(from));
            currentCallPeer = from;
            startVoiceChat(); // Окно открывается у инициатора
        } else {
            QMessageBox::information(this, "Вызов отклонен",
                QString("Пользователь %1 отклонил ваш вызов.").arg(from));
        }
    }else if (action == "ended") {
        // Обработка завершения вызова другой стороной
        QString from = callData["from"].toString();
        QMessageBox::information(this, "Вызов завершен",
            QString("Пользователь %1 завершил голосовой вызов.").arg(from));

        stopVoiceChat(); // Завершаем вызов и у себя
    }

}
void OKNO::startVoiceChat() {
    if (isInCall) return; // Уже в вызове

    isInCall = true;

    // Создаем диалог голосового чата (не модальный)
    voiceChatDialog = new QDialog(this);
    voiceChatDialog->setWindowTitle("Голосовой чат с " + currentCallPeer);
    voiceChatDialog->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(voiceChatDialog);
    QLabel *statusLabel = new QLabel("Идет разговор...", voiceChatDialog);
    QPushButton *endButton = new QPushButton("Завершить", voiceChatDialog);

    layout->addWidget(statusLabel);
    layout->addWidget(endButton);
    voiceChatDialog->setLayout(layout);

    connect(endButton, &QPushButton::clicked, this, &OKNO::stopVoiceChat);
    connect(voiceChatDialog, &QDialog::finished, this, &OKNO::stopVoiceChat);

    voiceChatDialog->show(); // Показываем немодально

    // Настройка аудио устройства
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use nearest";
        format = info.nearestFormat(format);
    }

    // Инициализация аудио устройств
    audioInput = new QAudioInput(format, this);
    audioOutput = new QAudioOutput(format, this);

    // Создаем буферы для аудио данных
    inputBuffer = new QBuffer(this);
    inputBuffer->open(QIODevice::ReadWrite);
    outputBuffer = new QBuffer(this);
    outputBuffer->open(QIODevice::ReadWrite);

    // Настройка UDP сокета для передачи голоса
    voiceSocket->bind(QHostAddress::Any, 12345);

    // Подключаем обработчик входящих данных
    connect(voiceSocket, &QUdpSocket::readyRead, this, &OKNO::processVoiceData);

    // Начинаем запись и воспроизведение
    audioInput->start(inputBuffer);
    audioOutput->start(outputBuffer);

    // Таймер для отправки аудио данных
    QTimer *sendTimer = new QTimer(this);
    connect(sendTimer, &QTimer::timeout, this, &OKNO::sendVoiceData);
    sendTimer->start(50); // Отправляем данные каждые 50 мс
}

void OKNO::processVoiceData() {
    while (voiceSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(voiceSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        voiceSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // Просто воспроизводим полученные данные (эхо)
        outputBuffer->seek(0);
        outputBuffer->write(datagram);
        outputBuffer->seek(0);
    }
}

void OKNO::sendVoiceData() {
    if (!isInCall) return;

    // Читаем данные из входного буфера
    QByteArray data = inputBuffer->readAll();
    if (data.isEmpty()) return;

    // Отправляем данные на другой конец (в реальном приложении нужно указать правильный адрес)
    // Здесь просто эмулируем отправку - в реальности нужно знать IP и порт собеседника
    voiceSocket->writeDatagram(data, QHostAddress::LocalHost, 12345);

    // Очищаем буфер для новых данных
    inputBuffer->seek(0);
    inputBuffer->buffer().clear();
}

void OKNO::stopVoiceChat() {
    if (!isInCall) return;

    isInCall = false;

    // Останавливаем аудио устройства
    if (audioInput) {
        audioInput->stop();
        delete audioInput;
        audioInput = nullptr;
    }

    if (audioOutput) {
        audioOutput->stop();
        delete audioOutput;
        audioOutput = nullptr;
    }

    // Закрываем буферы
    if (inputBuffer) {
        inputBuffer->close();
        delete inputBuffer;
        inputBuffer = nullptr;
    }

    if (outputBuffer) {
        outputBuffer->close();
        delete outputBuffer;
        outputBuffer = nullptr;
    }

    // Отправляем уведомление о завершении вызова
    if(!currentCallPeer.isEmpty()) {
        QJsonObject message;
        message["command"] = "VOICE_CALL_END";
        message["from"] = m_setting->getCurrentUsername();
        message["to"] = currentCallPeer;
        socketObj->write(QJsonDocument(message).toJson());
    }

    currentCallPeer.clear();

    // Закрываем диалог, если он еще открыт
    if (voiceChatDialog) {
        voiceChatDialog->close();
        voiceChatDialog = nullptr;
    }
}
OKNO::~OKNO() {
    delete socketObj;
}
