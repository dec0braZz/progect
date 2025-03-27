#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QCloseEvent>

class SecondWindow : public QDialog {
    Q_OBJECT
public:
    explicit SecondWindow(QWidget *parent = nullptr);

    void openSecondWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void closed();
};


#endif // SECONDWINDOW_H
