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
      movie(new QMovie("C:/Users/home/Desktop/qq/icons/ipone.gif")),
      movieGrupa(new QMovie("C:/Users/home/Desktop/qq/icons/grupa.gif")),
      timer(new QTimer(this))
{
    // Проверка загрузки анимаций
    if (!movie->isValid() || !movieGrupa->isValid()) {
        qDebug() << "Ошибка загрузки GIF-файлов!";
    }

    // Настройки окна
    setWindowTitle("KLOZ");
    setStyleSheet("background-color: gray;");
    resize(1280, 640);

    // Настройки для 'mesage'
    mesage->setGeometry(500, 70, 120, 45);
    mesage->setStyleSheet("border-radius: 40px;");
    mesage->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/ipone.gif"));
    mesage->setAlignment(Qt::AlignCenter);
    mesage->setScaledContents(true);
    mesage->installEventFilter(this);

    // Настройки для 'grupa'
    grupa->setGeometry(500, 130, 120, 45);
    grupa->setStyleSheet("border-radius: 40px;");
    grupa->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/grupa.gif"));
    grupa->setAlignment(Qt::AlignCenter);
    grupa->setScaledContents(true);
    grupa->installEventFilter(this);

    // Анимация для mesage
    auto animationToFinal = new QPropertyAnimation(mesage, "geometry");
    animationToFinal->setDuration(1000);
    animationToFinal->setStartValue(QRect(500, 70, 120, 45));
    animationToFinal->setEndValue(QRect(50, 30, 120, 45));

    // Анимация для grupa
    auto animationToFinalGrupa = new QPropertyAnimation(grupa, "geometry");
    animationToFinalGrupa->setDuration(1000);
    animationToFinalGrupa->setStartValue(QRect(500, 130, 120, 45));
    animationToFinalGrupa->setEndValue(QRect(50, 120, 120, 45));


    QTimer::singleShot(3000, [=]() {
        animationToFinal->start(QAbstractAnimation::DeleteWhenStopped);
        animationToFinalGrupa->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(timer, &QTimer::timeout, this, &OKNO::onTimerTimeout);
}


bool OKNO::eventFilter(QObject *obj, QEvent *event) {
    if (obj == mesage || obj == grupa) {
        if (event->type() == QEvent::Enter) {
            QLabel *target = qobject_cast<QLabel*>(obj);
            target->setMovie(obj == mesage ? movie : movieGrupa);
            movie->start();
            movieGrupa->start();
            timer->stop();
        } else if (event->type() == QEvent::Leave) {
            timer->start(2000);
        } else if (event->type() == QEvent::MouseButtonPress) {
            close();
        }
    }
    QWidget::eventFilter(obj, event);
}
OKNO::~OKNO() {
    delete movie;
    delete movieGrupa;
}

void OKNO::onTimerTimeout() {
    movie->stop();
    movieGrupa->stop();
    mesage->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/ipone.gif"));
    grupa->setPixmap(QPixmap("C:/Users/home/Desktop/qq/icons/grupa.gif"));
    timer->stop();
}

