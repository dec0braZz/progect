#include "MyServer.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QCryptographicHash>
#include<QFile>
#include <QTimer>
MyServer::MyServer(QObject *parent) : QTcpServer(parent) {
    connect(this, &MyServer::newConnection, this, &MyServer::onNewConnection);
    initDatabase();
}

void MyServer::initDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");

    if (!db.open()) {
        qCritical() << "Database error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec(R"(CREATE TABLE IF NOT EXISTS users (
        username TEXT PRIMARY KEY,
        password TEXT NOT ,
        email TEXT NOT ))");
    // Добавляем поле prefix
    query.exec("ALTER TABLE users ADD COLUMN prefix TEXT");
    //для пользовательских настроик
    query.exec(R"(CREATE TABLE IF NOT EXISTS user_settings (
           username TEXT,
           backgroundColor TEXT,
           textColor TEXT,
           FOREIGN KEY (username) REFERENCES users(username) ON DELETE CASCADE,
           PRIMARY KEY (username))");
    //ошибся надо пересоздать
    query.exec(R"(CREATE TABLE friend_requests (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        from_username TEXT NOT NULL,
        from_prefix TEXT NOT NULL,
        to_username TEXT NOT NULL,
        to_prefix TEXT NOT NULL,
        status TEXT DEFAULT 'pending'
    ))");

    // Добавляем таблицу уведомлений
        query.exec(R"(CREATE TABLE IF NOT EXISTS notifications (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            receiver_username TEXT NOT NULL,
            sender_username TEXT NOT NULL,
            message TEXT NOT NULL,
            read_status BOOLEAN DEFAULT FALSE
        ))");
        query.exec(R"(CREATE TABLE IF NOT EXISTS voice_calls (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            caller_username TEXT NOT NULL,
            callee_username TEXT NOT NULL,
            status TEXT DEFAULT 'pending',
            timestamp TEXT NOT NULL,
            UNIQUE(caller_username, callee_username) ON CONFLICT REPLACE
        ))");
    if (query.lastError().isValid()) {
        qCritical() << "Create table error:" << query.lastError().text();
    }
}

void MyServer::onNewConnection() {
    QTcpSocket *socket = nextPendingConnection();
    // username нужно получить из авторизации, поэтому временно сохраняем сокет
        userSockets["temp"] = socket; // временно сохраняем сокет
    connect(socket, &QTcpSocket::readyRead, [=]() { handleRequest(socket); });
    connect(socket, &QTcpSocket::disconnected, this, &MyServer::onSocketDisconnected);

    qDebug() << socket->state();
    qDebug() << socket->localAddress().toString();
    qDebug() << "Новое подключение " << socket->peerAddress().toString();
    qDebug() << "Connect -----------------------";
}

// !!!Добавить хеширование паролей (bcrypt/scrypt)!!!
bool MyServer::registerUser(const QString &username, const QString &password, const QString &email) {
    if (username.isEmpty() || password.isEmpty() || email.isEmpty()) {
        qDebug() << "Username, password, or email is empty.";
        return false;
    }

    // Проверка на существование пользователя
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (checkQuery.exec()) {
        checkQuery.next();
        if (checkQuery.value(0).toInt() > 0) {
            qDebug() << "Username already exists:" << username;
            return false; // именем пользователя уже занято
        }
    } else {
        qCritical() << "Error checking username:" << checkQuery.lastError().text();
        return false;
    }
    // Генерируем префикс
        QString prefix = generatePrefix();
    // Если имя пользователя новое, пытаемся зарегистрировать
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, email, prefix) VALUES (:username, :password, :email, :prefix)");
        query.bindValue(":username", username);
        query.bindValue(":password", password); // Хешируем пароль
        query.bindValue(":email", email);
        query.bindValue(":prefix", prefix);

    if (!query.exec()) {
        qCritical() << "Registration error:" << query.lastError().text();
        return false;
    }

    qDebug() << "User registered successfully:" << username;
    return true;
}
//Метод для сохранения  настроек
bool MyServer::saveUserSettings(const QString &username, const QString &backgroundColor, const QString &textColor) {
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO user_settings (username, backgroundColor, textColor) VALUES (:username, :backgroundColor, :textColor)");
    query.bindValue(":username", username);
    query.bindValue(":backgroundColor", backgroundColor);
    query.bindValue(":textColor", textColor);

    if (!query.exec()) {
        qCritical() << "Error saving user settings:" << query.lastError().text();
        return false;
    }
    return true;
}
//Методы  загрузки настроек
QJsonObject MyServer::loadUserSettings(const QString &username) {
    QJsonObject settings;

    QSqlQuery query;
    query.prepare("SELECT backgroundColor, textColor FROM user_settings WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        settings["backgroundColor"] = query.value(0).toString();
        settings["textColor"] = query.value(1).toString();
        qDebug() << "bg color from server:" << settings["backgroundColor"].toString();
    } else {
        qCritical() << "Error loading user settings:" << query.lastError().text();
    }
    return settings;
}


bool MyServer::authenticate(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :user");
    query.bindValue(":user", username);

    if (!query.exec() || !query.next()) {
        return false;
    }

    QString dbPassword = query.value(0).toString(); // Сравнение введенного пароля со значением из БД
    return (dbPassword == password);
}
// Получение prefix пользователя по username
QString MyServer::getPrefix(const QString &username) {
    QSqlQuery query(db);
    query.prepare("SELECT prefix FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        qCritical() << "Ошибка запроса prefix:" << query.lastError().text();
        return {};
    }
    if (query.next()) {
        return query.value(0).toString();
    }
    return {};
}

void MyServer::handleRequest(QTcpSocket *socket) {
    if (!socket->isOpen()) {
        qWarning() << "Socket is not open. Can't process request.";
        return;
    }

    QByteArray data = socket->readAll();
    QString command = QString::fromUtf8(data).trimmed();
    qDebug() << command;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        socket->write("ERROR: Invalid JSON format\n");
        return;
    }

    QJsonObject json = doc.object();
    QJsonObject jsonResponse;

    if (json["command"].toString() == "REGISTER" && json.contains("username") && json.contains("password") && json.contains("email")) {
        jsonResponse = registerHandler(json);
    } else if (json["command"].toString() == "LOGIN" && json.contains("username") && json.contains("password")) {
   //   void loginHandler(const QJsonObject& json);
        jsonResponse = loginHandler(json, socket);
    } else if (json["command"].toString() == "SAVE_SETTINGS" && json.contains("username") && json.contains("backgroundColor") && json.contains("textColor")) {
        jsonResponse = saveSettingsHandler(json);
    } else if (json["command"].toString() == "LOAD_SETTINGS" && json.contains("username")) {
        jsonResponse = loadSettingsHandler(json);
    } else if (json["command"].toString() == "SEND_FRIEND_REQUEST" && json.contains("from") && json.contains("to")) {
        jsonResponse = sendFriendRequestHandler(json); 
    } else if (json["command"].toString() == "ACCEPT_FRIEND_REQUEST" && json.contains("request_id")) {
        jsonResponse = acceptFriendRequestHandler(json);
    } else if (json["command"].toString() == "REJECT_FRIEND_REQUEST" && json.contains("request_id")) {
        jsonResponse = rejectFriendRequestHandler(json);
    }    else if (json["command"].toString() == "VOICE_CALL_REQUEST" && json.contains("from") && json.contains("to")) {
        jsonResponse = voiceCallRequestHandler(json);
    } else if (json["command"].toString() == "VOICE_CALL_RESPONSE" && json.contains("from") && json.contains("to") && json.contains("response")) {
        jsonResponse = voiceCallResponseHandler(json);
    } else if (json["command"].toString() == "VOICE_CALL_END" && json.contains("from") && json.contains("to")) {
        jsonResponse = voiceCallEndHandler(json);
    }
    else {
        jsonResponse = errorHandler();
    }

    QJsonDocument jsonDoc(jsonResponse);
    socket->write(jsonDoc.toJson());
    socket->flush();
}

QJsonObject MyServer::voiceCallRequestHandler(QJsonObject json) {
    QJsonObject jsonResponse;

    QString from = json["from"].toString();
    QString to = json["to"].toString();

    // Проверяем, существует ли пользователь, которому звоним
    QSqlQuery checkUser;
    checkUser.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkUser.bindValue(":username", to);

    if (!checkUser.exec() || !checkUser.next() || checkUser.value(0).toInt() == 0) {
        jsonResponse["status"] = "ERROR";
        jsonResponse["message"] = "User not found";
        return jsonResponse;
    }

    // Проверяем, онлайн ли пользователь
    QTcpSocket* recipientSocket = userSockets.value(to, nullptr);
    if (!recipientSocket) {
        jsonResponse["status"] = "ERROR";
        jsonResponse["message"] = "User is offline";
        return jsonResponse;
    }

    // Сохраняем запрос о вызове в базу данных
    QSqlQuery insertCall;
    insertCall.prepare(R"(
        INSERT INTO voice_calls (caller_username, callee_username, status, timestamp)
        VALUES (:caller, :callee, 'pending', datetime('now'))
    )");
    insertCall.bindValue(":caller", from);
    insertCall.bindValue(":callee", to);

    if (!insertCall.exec()) {
        qCritical() << "Error inserting voice call record:" << insertCall.lastError().text();
        jsonResponse["status"] = "ERROR";
        jsonResponse["message"] = "Database error";
        return jsonResponse;
    }

    // Получаем ID созданного вызова
    QSqlQuery idQuery("SELECT last_insert_rowid()");
    int callId = -1;
    if (idQuery.next()) {
        callId = idQuery.value(0).toInt();
    }

    // Отправляем уведомление о вызове
    QJsonObject callNotification;
    callNotification["command"] = "voice_call";
    callNotification["action"] = "incoming";
    callNotification["from"] = from;
    callNotification["from_prefix"] = getPrefix(from);
    callNotification["call_id"] = callId;  // Добавляем ID вызова

    recipientSocket->write(QJsonDocument(callNotification).toJson());
    recipientSocket->flush();

    jsonResponse["status"] = "SUCCESS";
    jsonResponse["message"] = "Call request sent";
    jsonResponse["call_id"] = callId;
    return jsonResponse;
}

QJsonObject MyServer::voiceCallResponseHandler(QJsonObject json) {
    QJsonObject jsonResponse;

    QString from = json["from"].toString();
    QString to = json["to"].toString();
    QString response = json["response"].toString(); // "accept" или "reject"

    // Обновляем статус вызова в базе данных
    QSqlQuery updateCall;
    updateCall.prepare(R"(
        UPDATE voice_calls
        SET status = :status
        WHERE caller_username = :caller AND callee_username = :callee AND status = 'pending'
    )");
    updateCall.bindValue(":status", response);
    updateCall.bindValue(":caller", to);  // Здесь to - это caller, так как ответ идет от callee
    updateCall.bindValue(":callee", from);

    if (!updateCall.exec()) {
        qCritical() << "Error updating voice call status:" << updateCall.lastError().text();
    }

    // Находим сокет вызывающего пользователя
    QTcpSocket* callerSocket = userSockets.value(to, nullptr);
    if (!callerSocket) {
        jsonResponse["status"] = "ERROR";
        jsonResponse["message"] = "Caller is now offline";
        return jsonResponse;
    }

    // Отправляем ответ вызывающему пользователю
    QJsonObject callResponse;
    callResponse["command"] = "voice_call";
    callResponse["action"] = "response";
    callResponse["from"] = from;
    callResponse["response"] = response;

    callerSocket->write(QJsonDocument(callResponse).toJson());
    callerSocket->flush();

    jsonResponse["status"] = "SUCCESS";
    jsonResponse["message"] = "Call response sent";
    return jsonResponse;
}

void MyServer::onSocketDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        // Удаляем пользователя из списка подключенных
        for (auto it = userSockets.begin(); it != userSockets.end(); ++it) {
            if (it.value() == socket) {
                userSockets.erase(it);
                break;
            }
        }
    }
}


// HANDLERS //

QJsonObject MyServer::errorHandler() {
    QJsonObject jsonResponse;
    jsonResponse["status"] = "ERROR";
    jsonResponse["message"] = "Unknown command or missing parameters.";
}

QJsonObject MyServer::registerHandler(QJsonObject json) {
    QJsonObject jsonResponse;

    QString username = json["username"].toString();
    QString password = json["password"].toString();
    QString email = json["email"].toString();

    // Регистрация пользователя
    if (json.contains("username") && json.contains("password") && json.contains("email")
        and registerUser(username, password, email)) {
        jsonResponse["status"] = "SUCCESS";
        jsonResponse["message"] = "User registered successfully!";
        return jsonResponse;
    }

    return  errorHandler();
}

QJsonObject MyServer::loginHandler(QJsonObject json, QTcpSocket *socket) {
    QJsonObject jsonResponse;

    QString username = json["username"].toString();
    QString password = json["password"].toString();

    // Аутентификация пользователя
    if (authenticate(username, password)) {
        jsonResponse["status"] = "SUCCESS";
        jsonResponse["message"] = "Login successful.";
        // Сохраняем сокет пользователя
        userSockets.remove("temp");
        userSockets[username] = socket;

        // Проверяем непрочитанные уведомления
        QTimer::singleShot(1000, [this, username, socket]() {
        QSqlQuery unreadNotifies;
        unreadNotifies.prepare("SELECT * FROM notifications WHERE receiver_username = :username AND read_status = FALSE");
        unreadNotifies.bindValue(":username", username);
        unreadNotifies.exec();

        while (unreadNotifies.next()) {
            QJsonObject notify;
            notify["command"] = "friend_request";
            notify["action"] = "incoming";
            notify["sender_username"] = unreadNotifies.value("sender_username").toString();
            socket->write(QJsonDocument(notify).toJson());

            // Отмечаем уведомление как прочитанное
            QSqlQuery markAsRead;
            markAsRead.prepare("UPDATE notifications SET read_status = TRUE WHERE id = :id");
            markAsRead.bindValue(":id", unreadNotifies.value("id").toInt());
            markAsRead.exec();
        }
        });
        return jsonResponse;
    }
    return errorHandler();
}

QJsonObject MyServer::saveSettingsHandler(QJsonObject json) {
    QJsonObject jsonResponse;

    QString username = json["username"].toString();
    QString backgroundColor = json["backgroundColor"].toString();
    QString textColor = json["textColor"].toString();

    if (saveUserSettings(username, backgroundColor, textColor)) {
        jsonResponse["status"] = "Setting load";
        jsonResponse["message"] = "User settings saved successfully!";
        return jsonResponse;
    } 
    return errorHandler();
}

QJsonObject MyServer::loadSettingsHandler(QJsonObject json) {
    QJsonObject jsonResponse;

    QString username = json["username"].toString();
    QJsonObject settings = loadUserSettings(username);

    if (!settings.isEmpty()) {
        jsonResponse["Setting load"] = "SUCCESS";
        jsonResponse["settings"] = settings;
        return jsonResponse;
    } 
    return errorHandler();
}

bool MyServer::validateFriendRequest(QJsonObject* jsonResponse,
        QString fromPrefix, QString toPrefix,
        QString from, QString to) {
    // Проверяем существование пользователей
    QSqlQuery checkFrom;
    checkFrom.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkFrom.bindValue(":username", from);

    QSqlQuery checkTo;
    checkTo.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkTo.bindValue(":username", to);

    // Сначала выполняем запросы
    if (!checkFrom.exec()) {
        qCritical() << "Error checking from user:" << checkFrom.lastError().text();
        (*jsonResponse)["status"] = "ERROR";
        (*jsonResponse)["message"] = "Error checking from user";
        return false;
    }

    if (!checkTo.exec()) {
        qCritical() << "Error checking to user:" << checkTo.lastError().text();
        (*jsonResponse)["status"] = "ERROR";
        (*jsonResponse)["message"] = "Error checking to user";
        return false;
    }

    // Теперь переходим к результатам
    checkFrom.next();
    checkTo.next();

    // Проверяем, существуют ли пользователи
    if (checkFrom.value(0).toInt() == 0) {
        (*jsonResponse)["status"] = "ERROR";
        (*jsonResponse)["message"] = "Отправляющий пользователь не найден";
        return false;
    }

    if (checkTo.value(0).toInt() == 0) {
        (*jsonResponse)["status"] = "ERROR";
        (*jsonResponse)["message"] = "Получающий пользователь не найден";
        return false;
    }
    // Проверяем, есть ли уже от этого пользователя незавершенный запрос дружбы
    QSqlQuery checkExisting;
    checkExisting.prepare(R"(
        SELECT COUNT(*) FROM friend_requests
        WHERE from_username = :from_username
        AND to_username = :to_username
        AND status = 'pending'
    )");
    checkExisting.bindValue(":from_username", from);
    checkExisting.bindValue(":to_username", to);

    if (!checkExisting.exec() || !checkExisting.next()) {
        qCritical() << "Error checking existing friend request:" << checkExisting.lastError().text();
        (*jsonResponse)["status"] = "ERROR";
        (*jsonResponse)["message"] = "Error checking existing request";
        return false;
    }

    if (checkExisting.value(0).toInt() > 0) {
        (*jsonResponse)["status"] = "ERROR";
        (*jsonResponse)["message"] = "Friend request already exists";
        return false;
    }

    return true;
}

QJsonObject MyServer::sendFriendRequestHandler(QJsonObject json) {
    QJsonObject jsonResponse;
    // Получаем префиксы для обоих пользователей
    QString from = json["from"].toString();
    QString to = json["to"].toString();
    QString fromPrefix = getPrefix(from);
    QString toPrefix = getPrefix(to);

    if (!validateFriendRequest(&jsonResponse, fromPrefix, toPrefix, from, to))
        return jsonResponse;

    // Добавляем запрос в базу данных
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO friend_requests (from_username, from_prefix, to_username, to_prefix, status)
        VALUES (:from_username, :from_prefix, :to_username, :to_prefix, 'pending')
    )");
    query.bindValue(":from_username", from);
    query.bindValue(":from_prefix", fromPrefix);
    query.bindValue(":to_username", to);
    query.bindValue(":to_prefix", toPrefix);

    if (!query.exec()) {
        qCritical() << "Error inserting friend request:" << query.lastError().text();
        return errorHandler();
    } else {
        jsonResponse["status"] = "SUCCESS";
        jsonResponse["message"] = "Friend request sent";
        // Получаем ID нового запроса
        QSqlQuery idQuery("SELECT last_insert_rowid()");
        int requestId = -1;
        if (idQuery.next()) {
            requestId = idQuery.value(0).toInt();
        }

        // Находим сокет получателя
        QTcpSocket* recipientSocket = userSockets.value(to, nullptr);
        // Новая логика: если получатель не онлайн - сохраняем уведомление
        if (!recipientSocket) {
            QSqlQuery notifyQuery;
            notifyQuery.prepare("INSERT INTO notifications (receiver_username, sender_username, message) "
                                "VALUES (:receiver, :sender, :message)");
            notifyQuery.bindValue(":receiver", to);
            notifyQuery.bindValue(":sender", from);
            notifyQuery.bindValue(":message", "Friend request");
            notifyQuery.exec();
        }

        // Если получатель онлайн - отправляем уведомление
        if (recipientSocket) {
            QJsonObject notify;
            notify["command"] = "friend_request";
            notify["action"] = "incoming";
            notify["request_id"] = QString::number(requestId);
            notify["sender_username"] = from;
            notify["sender_prefix"] = fromPrefix;

            recipientSocket->write(QJsonDocument(notify).toJson());
        }

        return jsonResponse;
    }
}

QJsonObject MyServer::acceptFriendRequestHandler(QJsonObject json) {
    QJsonObject jsonResponse;

    QString requestId = json["request_id"].toString();

    // Обновляем статус запроса
    QSqlQuery query;
    query.prepare("UPDATE friend_requests SET status = 'accepted' "
                 "WHERE id = :request_id");
    query.bindValue(":request_id", requestId);

    if (!query.exec()) {
        jsonResponse["status"] = "ERROR";
        jsonResponse["message"] = "Failed to accept request";
    } else {
        jsonResponse["status"] = "SUCCESS";
        jsonResponse["message"] = "Friend request accepted";
        // Получаем данные о запросе
       QSqlQuery requestQuery;
       requestQuery.prepare("SELECT from_username, to_username "
                           "FROM friend_requests "
                           "WHERE id = :request_id");
       requestQuery.bindValue(":request_id", requestId);
       if (requestQuery.exec() && requestQuery.next()) {
           QString sender = requestQuery.value(0).toString();
           QString receiver = requestQuery.value(1).toString();

           // Уведомляем отправителя
           QTcpSocket* senderSocket = userSockets.value(sender, nullptr);
           if (senderSocket) {
               QJsonObject notify;
               notify["command"] = "friend_request";
               notify["action"] = "accepted";
               notify["friend_username"] = receiver;
               senderSocket->write(QJsonDocument(notify).toJson());
           }

           // Уведомляем получателя
           QTcpSocket* receiverSocket = userSockets.value(receiver, nullptr);
           if (receiverSocket) {
               QJsonObject notify;
               notify["command"] = "friend_request";
               notify["action"] = "accepted";
               notify["friend_username"] = sender;
               receiverSocket->write(QJsonDocument(notify).toJson());
           }
        }
    }

    return jsonResponse;
}

QJsonObject MyServer::rejectFriendRequestHandler(QJsonObject json) {
    QJsonObject jsonResponse;;

    QString requestId = json["request_id"].toString();

    // Удаляем запрос из базы данных
    QSqlQuery query;
    query.prepare("DELETE FROM friend_requests WHERE id = :request_id");
    query.bindValue(":request_id", requestId);

    if (!query.exec()) {
        jsonResponse["status"] = "ERROR";
        jsonResponse["message"] = "Failed to reject request";
    } else {
        jsonResponse["status"] = "SUCCESS";
        jsonResponse["message"] = "Friend request rejected";

        // Получаем данные о отклоненном запросе
        QSqlQuery requestQuery;
        requestQuery.prepare("SELECT from_username "
                            "FROM friend_requests "
                            "WHERE id = :request_id");
        requestQuery.bindValue(":request_id", requestId);

        if (requestQuery.exec() && requestQuery.next()) {
            QString sender = requestQuery.value(0).toString();

            // Уведомляем отправителя о том, что его запрос отклонен
            QTcpSocket* senderSocket = userSockets.value(sender, nullptr);
            if (senderSocket) {
                QJsonObject notify;
                notify["command"] = "friend_request";
                notify["action"] = "rejected";
                notify["sender_username"] = sender;
                senderSocket->write(QJsonDocument(notify).toJson());
            }
        }
    }

    return jsonResponse;
}
QJsonObject MyServer::voiceCallEndHandler(const QJsonObject &json) {
    QJsonObject jsonResponse;

    QString from = json["from"].toString();
    QString to = json["to"].toString();

    // Обновляем статус вызова в базе данных
    QSqlQuery updateCall;
    updateCall.prepare(R"(
        UPDATE voice_calls
        SET status = 'ended'
        WHERE (caller_username = :from AND callee_username = :to)
           OR (caller_username = :to AND callee_username = :from)
        AND status = 'accepted'
    )");
    updateCall.bindValue(":from", from);
    updateCall.bindValue(":to", to);

    if (!updateCall.exec()) {
        qCritical() << "Error updating voice call status:" << updateCall.lastError().text();
        jsonResponse["status"] = "ERROR";
        jsonResponse["message"] = "Database error";
        return jsonResponse;
    }

    // Находим сокет другого участника вызова
    QTcpSocket* otherUserSocket = userSockets.value(to, nullptr);
    if (otherUserSocket) {
        // Отправляем уведомление о завершении вызова
        QJsonObject endNotification;
        endNotification["command"] = "voice_call";
        endNotification["action"] = "ended";
        endNotification["from"] = from;

        otherUserSocket->write(QJsonDocument(endNotification).toJson());
        otherUserSocket->flush();
    }

    jsonResponse["status"] = "SUCCESS";
    jsonResponse["message"] = "Call ended successfully";
    return jsonResponse;
}
