#include "secondwindowregistr.h"
#include <QDebug>

SecondWindowRegistr::SecondWindowRegistr(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Регистрация");
    resize(300, 200); // Устанавливаем размер окна
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Элементы ввода
    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("Введите ваш e-mail");
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Введите логин");
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Введите пароль");
    passwordEdit->setEchoMode(QLineEdit::Password); // Скрытие пароля

    // Добавляем элементы на форму
    layout->addWidget(new QLabel("E-Mail:"));
    layout->addWidget(emailEdit);
    layout->addWidget(new QLabel("Логин:"));
    layout->addWidget(usernameEdit);
    layout->addWidget(new QLabel("Пароль:"));
    layout->addWidget(passwordEdit);

    // Кнопка регистрации
    QPushButton *registerButton = new QPushButton("Зарегистрироваться", this);
    layout->addWidget(registerButton);

    // Подключаем сигнал кнопки
    connect(registerButton, &QPushButton::clicked, [this]() {
        QString email = emailEdit->text();
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();

        // Проверка ввода
        if (email.isEmpty() || username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
            return;
        }
        sendRegistrationData(email, username,password);
        QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");
        this->accept(); // Закрыть окно после успешной регистрации

    });

    setLayout(layout); // Устанавливаем layout

}
void SecondWindowRegistr::sendRegistrationData(const QString &email, const QString &username, const QString &password) {
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    // Создаем JSON объект для отправки на сервер
    QJsonObject json;
    json["command"] = "REGISTER";
    json["email"] = email;
    json["username"] = username;
    json["password"] = password;

    // Преобразуем JSON объект в QByteArray
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();
    QString mesage = QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    QUrl url("http://127.0.0.1:1234/register"); // URL вашего локального сервера
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "Отправлен соо на сервер" << QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
    // Отправляем POST запрос с JSON данными
       networkManager->post(request, jsonData);
}

