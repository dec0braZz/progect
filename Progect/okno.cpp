#include "okno.h"
#include <QEvent>
#include <QMovie>
#include <QTimer>
#include <QWidget>
#include <QPropertyAnimation>
#include <QDebug>

OKNO::OKNO(QWidget *parent)
    : QWidget(parent),
      mesage(new QLabel(this)),
      grupa(new QLabel(this)),
      seting(new QLabel(this)),
      profile(new QLabel(this)),
      movie(new QMovie("C:/Users/home/Desktop/qq/icons/ipone.gif")),
      movieGrupa(new QMovie("C:/Users/home/Desktop/qq/icons/grupa.gif")),
      movieSeting(new QMovie("C:/Users/home/Desktop/qq/icons/seting.gif")),
      movieProfile(new QMovie("C:/Users/home/Desktop/qq/icons/profile.gif")),
      timer(new QTimer(this)) {

    // Проверка загрузки анимаций
    if (!movie->isValid() || !movieGrupa->isValid() || !movieSeting->isValid()) {
        qDebug() << "Ошибка загрузки GIF-файлов!";
    }

    // Настройки окна
    setWindowTitle("KLOZ");
    setStyleSheet("background-color: grey;");
    resize(1280, 640);

    //Настройка для "profile"
    profile->setGeometry(500, 250, 120, 45);
    profile->setStyleSheet("border-radius: 40px;");
    profile->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/profile.gif"));
    profile->setAlignment(Qt::AlignCenter);
    profile->setScaledContents(true);
    profile->installEventFilter(this);
    // Настройки для 'mesage' Hover Фрейм
    mesage->setGeometry(500, 70, 120, 45);
    mesage->setStyleSheet("border-radius: 40px;");
    mesage->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/ipone.gif"));
    mesage->setAlignment(Qt::AlignCenter);
    mesage->setScaledContents(true);
    mesage->installEventFilter(this);

    // Настройки для 'grupa'
    grupa->setGeometry(500, 130, 120, 45);
    grupa->setStyleSheet(mesage->styleSheet());
    grupa->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/grupa.gif"));
    grupa->setAlignment(Qt::AlignCenter);
    grupa->setScaledContents(true);
    grupa->installEventFilter(this);

    // Настройки для 'seting'
    seting->setGeometry(500, 190, 120, 45);
    seting->setStyleSheet("border-radius: 40px;");
    seting->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/seting.gif"));
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

    connect(timer, &QTimer::timeout, this, &OKNO::onTimerTimeout);
}

bool OKNO::eventFilter(QObject *obj, QEvent *event) {
    if (obj == mesage || obj == grupa || obj == seting || obj == profile) {
        if (event->type() == QEvent::Enter) {
            QLabel *target = qobject_cast<QLabel*>(obj);
            if (target) {
                if (obj == mesage) {
                    target->setMovie(movie);
                    movie->start();
                } else if (obj == grupa) {
                    target->setMovie(movieGrupa);
                    movieGrupa->start();
                } else if (obj == profile) {
                    target->setMovie(movieProfile);
                    movieProfile->start();
                } else if (obj == seting) {
                    target->setMovie(movieSeting);
                    movieSeting->start();
                }
                timer->stop();
            }
        } else if (event->type() == QEvent::Leave) {
            timer->start(2000); // Задержка 2 секунды после нажатия
        } else if (event->type() == QEvent::MouseButtonPress) {
            close();
        }
    }
    return QWidget::eventFilter(obj, event);
}

void OKNO::onTimerTimeout() {
    movie->stop();
    movieGrupa->stop();
    movieSeting->stop();
    movieProfile->stop();
    mesage->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/ipone.gif"));
    grupa->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/grupa.gif"));
    seting->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/seting.gif"));
    profile->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/profile.gif"));
}

OKNO::~OKNO() {
    delete movie;
    delete movieGrupa;
    delete movieSeting;
    delete movieProfile;
}
