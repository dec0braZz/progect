#ifndef OKNO_H
#define OKNO_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QPropertyAnimation>
#include "setting.h"
#include"ColorPalette.h"

class OKNO : public QWidget {
    Q_OBJECT
public:
    explicit OKNO(QWidget *parent = nullptr);
    void openOKNO();
    ~OKNO();
protected:
    void mousePressEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent *event) override{

    }
private: Setting* settingWindow;
private slots:
    void onButtonHovered(){

    }
    void onButtonClicked(){}

private:
    QLabel *mesage;
    QLabel *grupa;
    QLabel *seting;
    QLabel *profile;
    QTimer *timer;
    QPropertyAnimation *animation;
    QPropertyAnimation *animationToFinal;
        QPropertyAnimation *animationToFinalGrupa;
        QPropertyAnimation *animationToFinalseting;
        QPropertyAnimation *animationToFinalprofile;

signals:
    void closed();
};

#endif // OKNO_H
