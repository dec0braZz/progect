#ifndef OKNO_H
#define OKNO_H
#include <QUdpSocket>
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
     explicit OKNO(Setting* setting,QSharedPointer<QTcpSocket> socket, QWidget *parent = nullptr);
    void openOKNO();

    void openChatWindow(const QString &friendName);
    void processFriendResponse(const QJsonObject& response); //dataresiv
    ~OKNO();
protected:
    void closeEvent(QCloseEvent *event) override{

    }
private:
    QSharedPointer<QTcpSocket> tcpSocket;  // Для команд
        QUdpSocket* voiceSocket;
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

          QString currentCallPeer;
          bool isInCall;

public slots:
            void handleIncomingVoiceCall(const QJsonObject& callData);
  void updateWindowColors(const QColor& background, const QColor& text); //слот для обработки изменения цветов через setting
private slots:

  void startVoiceChat();
   void stopVoiceChat();
  void processVoiceCallResponse(const QJsonObject& response);
  void processIncomingCall(const QJsonObject& notification);
  void sendVoiceCallRequest(const QString &friendId);
  void sendFriendRequest(const QString& friendId);

signals:
    void openSettings();
    void closed();
};
class FriendWidget : public QWidget {
    Q_OBJECT
public:
    explicit FriendWidget(const QString &friendName, QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QHBoxLayout(this);
        auto *label = new QLabel(friendName, this);
        auto *chatButton = new QPushButton("Чат", this);
           auto *voiceButton = new QPushButton("Голос", this);
        layout->addWidget(label);
        layout->addStretch();
        layout->addWidget(chatButton);
          layout->addWidget(voiceButton);
        setLayout(layout);

        connect(chatButton, &QPushButton::clicked, this, [this, friendName](){
            emit chatRequested(friendName);
        });
        connect(voiceButton, &QPushButton::clicked, this, [this, friendName](){
                emit voiceCallRequested(friendName);
            });
    }

signals:
    void chatRequested(const QString &friendName);
     void voiceCallRequested(const QString &friendName);
};

#endif // OKNO_H
