#ifndef MYSERVER_H
#define MYSERVER_H
#include <QJsonArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUuid> // для хранения токенаЫ
#include <QRandomGenerator>
class MyServer : public QTcpServer {
    Q_OBJECT

public:
    explicit MyServer(QObject *parent = nullptr);
    void initDatabase();
    void sendJson(QTcpSocket *socket, const QJsonObject &obj);
    QJsonArray loadFriends(const QString& username);
    QJsonObject registerHandler(QJsonObject json);
    QJsonObject loginHandler(QJsonObject json, QTcpSocket *socket);
    QJsonObject saveSettingsHandler(QJsonObject json);
    QJsonObject loadSettingsHandler(QJsonObject json);
    QJsonObject sendFriendRequestHandler(QJsonObject json);
    bool validateFriendRequest(QJsonObject* jsonResponse,
            QString fromPrefix, QString toPrefix,
            QString from, QString to);
    QJsonObject acceptFriendRequestHandler(QJsonObject json);
    QJsonObject rejectFriendRequestHandler(QJsonObject json);


private slots:
    QJsonObject loadUserSettings(const QString &username);
   // void friend_request_timer();

    void onSocketDisconnected() ;
    bool saveUserSettings(const QString &username, const QString &backgroundColor, const QString &textColor);
    void onNewConnection();
    void handleRequest(QTcpSocket *socket);
    QJsonObject errorHandler();

private slots:
    QString generatePrefix() {
        QString prefix = "#";
        QString letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        QString digits = "0123456789";

        // Генерируем 2 случайные буквы
        for(int i = 0; i < 2; i++) {
            prefix += letters[QRandomGenerator::global()->bounded(0, letters.length())];
        }

        // Генерируем 3 случайные цифры
        for(int i = 0; i < 3; i++) {
            prefix += digits[QRandomGenerator::global()->bounded(0, digits.length())];
        }

        return prefix;
    }
QString getPrefix(const QString &username);
private:
    QSqlDatabase db;
    QMap<QString, QTcpSocket*> userSockets;
    QMap<QString, QString> sessionTokens; // для хранения токенов пользователей


    bool registerUser(const QString &username, const QString &password, const QString &email);
    bool authenticate(const QString &username, const QString &password);
};

#endif // MYSERVER_H
