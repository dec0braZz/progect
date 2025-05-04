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
#include <QListWidget>
#include <QDialog>
#include <QHBoxLayout>

class OKNO : public QWidget {
    Q_OBJECT
public:
     explicit OKNO(Setting* setting, QWidget *parent = nullptr);

    void openOKNO();
    ~OKNO();
protected:
    void closeEvent(QCloseEvent *event) override{

    }
private:
    Setting* m_setting;
private slots:
    void onButtonHovered(){

    }
    void onButtonClicked(){}
void showSetting();
void showGroupActions();//обьеденение
private:
    Socket *socketObj;
    QLabel *mesage;
    QLabel *grupa;
    QLabel *seting;
    QLabel *profile;
    QPropertyAnimation *animation;
    QPropertyAnimation *animationToFinal;
        QPropertyAnimation *animationToFinalGrupa;
        QPropertyAnimation *animationToFinalseting;
        QPropertyAnimation *animationToFinalprofile;
        QListWidget* friendsList;
            QListWidget* addFriendList;
signals:
    void openSettings();
    void closed();
};

#endif // OKNO_H
