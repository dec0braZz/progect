#include "secondwindow.h"
#include "okno.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

SecondWindow::SecondWindow(QWidget *parent)
    : QDialog(parent),
      socket(new QTcpSocket(this)),
      networkManager(new QNetworkAccessManager(this)) // Инициализируем networkManager
{
    setWindowTitle("Авторизация");
    resize(600, 400);

    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password); // Скрытие пароля
    statusLabel = new QLabel(this);

    QPushButton *loginButton = new QPushButton("Войти", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Имя пользователя:"));
    layout->addWidget(usernameEdit);
    layout->addWidget(new QLabel("Пароль:"));
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);

    connect(loginButton, &QPushButton::clicked, this, &SecondWindow::onLoginClicked);
    connect(socket, &QTcpSocket::readyRead, this, &SecondWindow::onDataReceived);
    connect(socket, &QTcpSocket::stateChanged, this, &SecondWindow::onStateChanged);

    // Подключаем к серверу
    socket->connectToHost("127.0.0.1", 1234);
}

void SecondWindow::onLoginClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    login(username, password);
}

void SecondWindow::login(const QString &username, const QString &password) {
    // Создаем JSON объект для отправки на сервер
    QJsonObject json;
    json["command"] = "LOGIN";
    json["username"] = username;
    json["password"] = password;

    // Преобразуем JSON объект в QByteArray
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    socket->write(jsonData);
    socket->flush();
}

void SecondWindow::onDataReceived() {
    QByteArray responseData = socket->readAll(); // Читаем данные от сервера
    qDebug() << "Received response:" << responseData;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (jsonDoc.isObject()) {
        QJsonObject jsonResponse = jsonDoc.object();
        if (jsonResponse["status"].toString() == "SUCCESS") {
            statusLabel->setText("Вход успешен");
            statusLabel->setStyleSheet("color: green;");

            // Показать следующее окно
            OKNO *okno = new OKNO();
            okno->show();
            this->accept();
        } else if (jsonResponse["status"].toString() == "ERROR") {
            statusLabel->setText("Неверное имя пользователя или пароль");
            statusLabel->setStyleSheet("color: red;");
        }
    } else {
        statusLabel->setText("Ошибка получения ответа от сервера.");
        statusLabel->setStyleSheet("color: red;");
    }
}

void SecondWindow::onStateChanged(QAbstractSocket::SocketState state) {
    switch (state) {
        case QAbstractSocket::UnconnectedState:
            qDebug() << "Состояние: UnconnectedState";
            break;
        case QAbstractSocket::HostLookupState:
            qDebug() << "Состояние: HostLookupState";
            break;
        case QAbstractSocket::ConnectingState:   qDebug() << "Состояние: ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << "Состояние: ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        qDebug() << "Состояние: BoundState";
        break;
    case QAbstractSocket::ListeningState:
        qDebug() << "Состояние: ListeningState";
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << "Состояние: ClosingState";
        break;
}
}

SecondWindow::~SecondWindow() {
delete socket; // Обязательно удаляем сокет
}

void SecondWindow::closeEvent(QCloseEvent *event) {
emit closed();
event->accept();
}
