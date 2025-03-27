#include "secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("AVTORISATION");
    resize(600, 400);
}

void SecondWindow::openSecondWindow() {
    this->exec();
}
void SecondWindow::closeEvent(QCloseEvent *event) {
    emit closed(); // Отправка сигнала при закрытии
    event->accept(); // Подтверждение закрытия
}

