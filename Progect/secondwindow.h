#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H
#include<QLabel>
#include<QVBoxLayout>
#include<QPushButton>
#include <QWidget>
#include <QDialog>
#include <QCloseEvent>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QLineEdit>
#include <okno.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>
class SecondWindow : public QDialog {
    Q_OBJECT
public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow(); // обьявления деструктора
    void login(const QString &username, const QString &password);
    void openSecondWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QTcpSocket *socket; // Для соединения с сервером
    QSqlDatabase db; // Для работы с базой данных
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLabel *statusLabel;

private slots:
    void onDataReceived();
    void onLoginClicked();
    void onStateChanged(QAbstractSocket::SocketState state);
signals:
    void closed();
};


#endif // SECONDWINDOW_H
