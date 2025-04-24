#include "okno.h"
#include"ColorPalette.h"
#include <QEvent>
#include <QMovie>
#include <QTimer>
#include <QWidget>
#include <QPropertyAnimation>
#include <QDebug>
#include "setting.h"
#include <QMouseEvent>
#include "socket.h"
const QString IPHONE_PATH = "C:/Users/home/Desktop/qq/icons/ipone.gif";
const QString GRUPA_PATH = "C:/Users/home/Desktop/qq/icons/grupa.gif";
const QString SETING_PATH = "C:/Users/home/Desktop/qq/icons/seting.gif";
const QString PROFILE_PATH = "C:/Users/home/Desktop/qq/icons/profile.gif";

OKNO::OKNO(Setting* setting, QWidget *parent) : QWidget(parent), m_setting(setting),
      mesage(new QLabel(this)),
      grupa(new QLabel(this)),
      seting(new QLabel(this)),
      profile(new QLabel(this)),
      timer(new QTimer(this)) {
     socketObj = new Socket(this);  // создаём объект Socket
    setWindowTitle("KLOZ");
    setStyleSheet("background-color: grey;");
    resize(1280, 640);

    // Настройка для "profile"
    profile->setGeometry(500, 250, 120, 45);
    profile->setStyleSheet("border-radius: 40px;");
    profile->setPixmap(QPixmap(PROFILE_PATH));
    profile->setAlignment(Qt::AlignCenter);
    profile->setScaledContents(true);
    profile->installEventFilter(this);

    // Настройки для 'mesage' Hover Фрейм
    mesage->setGeometry(500, 70, 120, 45);
    mesage->setStyleSheet("border-radius: 40px;");
    mesage->setPixmap(QPixmap(IPHONE_PATH));
    mesage->setAlignment(Qt::AlignCenter);
    mesage->setScaledContents(true);
    mesage->installEventFilter(this);

    // Настройки для 'grupa'
    grupa->setGeometry(500, 130, 120, 45);
    grupa->setStyleSheet("border-radius: 40px;");
    grupa->setPixmap(QPixmap(GRUPA_PATH));
    grupa->setAlignment(Qt::AlignCenter);
    grupa->setScaledContents(true);
    grupa->installEventFilter(this);

    // Настройки для 'seting'
    seting->setGeometry(500, 190, 120, 45);
    seting->setStyleSheet("border-radius: 40px;");
    seting->setPixmap(QPixmap(SETING_PATH));
    seting->setAlignment(Qt::AlignCenter);
    seting->setScaledContents(true);
    seting->installEventFilter(this);

    // Анимация
    auto animationToFinal = new QPropertyAnimation(mesage, "geometry");
    animationToFinal->setDuration(1000);
    animationToFinal->setStartValue(QRect(500, 70, 120, 45));
    animationToFinal->setEndValue(QRect(50, 30, 120, 45));

    auto animationToFinalGrupa = new QPropertyAnimation(grupa, "geometry");
    animationToFinalGrupa->setDuration(1000);
    animationToFinalGrupa->setStartValue(QRect(500, 130, 120, 45));
    animationToFinalGrupa->setEndValue(QRect(50, 120, 120, 45));

    auto animationToFinalseting = new QPropertyAnimation(seting, "geometry");
    animationToFinalseting->setDuration(1000);
    animationToFinalseting->setStartValue(QRect(500, 190, 120, 45));
    animationToFinalseting->setEndValue(QRect(50, 210, 120, 45));

    auto animationToFinalprofile = new QPropertyAnimation(profile, "geometry");
    animationToFinalprofile->setDuration(1500);
    animationToFinalprofile->setStartValue(QRect(500, 250, 120, 45));
    animationToFinalprofile->setEndValue(QRect(50, 500, 120, 45));

    QTimer::singleShot(3000, [=]() {
        animationToFinal->start(QAbstractAnimation::DeleteWhenStopped);
        animationToFinalGrupa->start(QAbstractAnimation::DeleteWhenStopped);
        animationToFinalseting->start(QAbstractAnimation::DeleteWhenStopped);
        animationToFinalprofile->start(QAbstractAnimation::DeleteWhenStopped);
    });

    timer->setSingleShot(true);
}
void OKNO::mousePressEvent(QMouseEvent *event) {
    //  проверяем, была ли нажата кнопка 'seting'
    if (event->button() == Qt::LeftButton) {
        if (seting->geometry().contains(event->pos())) {
            // Открываем окно настроек
                        m_setting->show();
        }
    }
    QWidget::mousePressEvent(event);
}

OKNO::~OKNO() {
}
