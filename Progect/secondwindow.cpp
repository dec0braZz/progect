#include "secondwindow.h"
#include "okno.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

SecondWindow::SecondWindow(QWidget *parent)
    : QDialog(parent),
      socket(new QTcpSocket(this))
{
    setWindowTitle("AVTORISATION");
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
    // Обработка ответа от сервера
    socket->connectToHost("127.0.0.1", 1234);
}

void SecondWindow::onLoginClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    login(username, password);
}

void SecondWindow::login(const QString &username, const QString &password) {
    QString data = QString("LOGIN %1 %2").arg(username).arg(password);
    socket->write(data.toUtf8());
    socket->flush();
}

void SecondWindow::onDataReceived() {
    QByteArray responseData = socket->readAll();
    QString response = QString::fromUtf8(responseData);

    if (response == "SUCCESS") {
        statusLabel->setText("Вход успешен");
        statusLabel->setStyleSheet("color: green;");
    //dbpassword = select password from users where login=''
        // if dbpassword == password_from_window
        // Показать следующее окно
        OKNO *okno = new OKNO();
        okno->show();
        this->accept();
    } else {
        statusLabel->setText("Ошибка входа. Попробуйте ещё раз.");
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
        case QAbstractSocket::ConnectingState:
            qDebug() << "Состояние: ConnectingState";
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

