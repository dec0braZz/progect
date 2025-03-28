#ifndef OKNO_H
#define OKNO_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QPropertyAnimation>

class OKNO : public QWidget {
    Q_OBJECT
public:
    explicit OKNO(QWidget *parent = nullptr);
    void openOKNO();
    ~OKNO();
protected:
    void closeEvent(QCloseEvent *event) override{

    }
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onButtonHovered(){

    }
    void onButtonClicked(){

    }
    void onTimerTimeout();

private:
    QLabel *mesage;
    QLabel *grupa;
    QLabel *seting;
    QLabel *profile;

    QMovie *movie;
    QMovie *movieGrupa;
    QMovie *movieSeting;
    QMovie *movieProfile;
    QTimer *timer;
    QPropertyAnimation *animation;
signals:
    void closed();
};

#endif // OKNO_H
