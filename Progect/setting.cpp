#include "setting.h"
#include "ColorPalette.h"

Setting::Setting(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Setting");
    setStyleSheet("background-color: grey;");
    resize(1280, 800);

    colorButton = new QPushButton("Открыть меню выбора цветов", this);
    colorButton->setGeometry(50, 100, 300, 50); // Положение и размер кнопки
    connect(colorButton, &QPushButton::clicked, this, &Setting::on_colorButton_clicked);

    colorPalette = nullptr; // Инициализация указателя на палитру
}

void Setting::openColorPalette() {
    if (!colorPalette) { // Если палитра еще не создана
        colorPalette = new ColorPalette(this);
        connect(colorPalette, &ColorPalette::colorsChanged, this, &Setting::updateColors);
    }
    colorPalette->show(); // Показать палитру
}

void Setting::updateColors(const QColor &background, const QColor &text) {
    // Устанавливаем цвет фона
    this->setStyleSheet(QString("background-color: %1;").arg(background.name()));
    // Если у вас есть элементы, которые нужно обновить, находите их и обновляйте цвет текста
    // Например, если у вас есть метка или кнопка
    // myLabel->setStyleSheet(QString("color: %1;").arg(text.name()));
}

void Setting::on_colorButton_clicked() {
    if (colorPalette && colorPalette->isVisible()) {
        // Если палитра открыта, закрываем её
        closeColorPalette();
    } else {
        // Если палитра закрыта, открываем её
        openColorPalette();
    }
}

void Setting::closeColorPalette() {
    if (colorPalette) {
        colorPalette->hide();
    }
}
