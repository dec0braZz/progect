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
    QMovie *movie;
    QMovie *movieGrupa;
    QTimer *timer;
    QPropertyAnimation *animation;

signals:
    void closed();
};

#endif // OKNO_H
