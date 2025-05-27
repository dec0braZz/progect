#ifndef OKNO_H
#define OKNO_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QPropertyAnimation>
#include "setting.h"
#include"ColorPalette.h"
#include <QTcpSocket>
#include <socket.h>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include<QMap>
#include <QListWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>

class OKNO : public QWidget {
    Q_OBJECT
public:
     explicit OKNO(Setting* setting, QWidget *parent = nullptr);
    void openOKNO();
    void processFriendResponse(const QJsonObject& response); //dataresiv
    ~OKNO();
protected:
    void closeEvent(QCloseEvent *event) override{

    }
private:
    Setting* m_setting;
private slots:
    void handleIncomingRequest(const QJsonObject& request);
    void rejectFriendRequest();
    void acceptFriendRequest();

    void onButtonHovered(){

    }
    void onButtonClicked(){}
void showSetting();
void showGroupActions();//обьеденение
void updatePendingRequestsList();
void updateFriendsList();
  void handleFriendsList(const QJsonArray& friends);
private:
    Socket *socketObj;
    QLabel *mesage;
    QLabel *grupa;
    QLabel *seting;
    QLabel *profile;

    QSharedPointer<QListWidget> friendsList;
    QSharedPointer<QListWidget> pendingRequestsList;// умные указатели добавление в друзья
    QMap<QString, QString> friends;// ключ - ID друга, значение - его имя
       QMap<QString, QString> pendingRequests; // ключ - ID запроса, значение - имя отправителя
        QPushButton *rejectButton;
        QPushButton *acceptButton;
        QLineEdit *friendIdInput;
        QPushButton *addFriendButton;

        // Для обработки входящих запросов
        QTimer *requestTimer;
        QMap<QString, QTimer*> requestTimers;
public slots:
  void updateWindowColors(const QColor& background, const QColor& text); //слот для обработки изменения цветов через setting
private slots:
  void sendFriendRequest(const QString& friendId);

signals:
    void openSettings();
    void closed();
};

#endif // OKNO_H
