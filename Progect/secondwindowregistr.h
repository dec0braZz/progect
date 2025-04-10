#ifndef SECONDWINDOWREGISTR_H
#define SECONDWINDOWREGISTR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject> //для того чтобы  документ баззы данных работал
#include <QString>
class SecondWindowRegistr : public QDialog {
    Q_OBJECT

public:
    explicit SecondWindowRegistr(QWidget *parent = nullptr);
    void sendRegistrationData(const QString &email, const QString &username, const QString &password);
private:
    QLineEdit *emailEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QString email;      // Email пользователя
    QString username;
    QString password;
};

#endif // SECONDWINDOWREGISTR_H
