#include "ColorPalette.h"
#include <QVariant>
ColorPalette::ColorPalette(QWidget* parent)
    : QWidget(parent),
    backgroundColor(Qt::white),  // Добавляем инициализацию
    textColor(Qt::black)
{
    setWindowTitle("Настройка палитры цветов");
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Кнопка для выбора фона
    QPushButton *backgroundColorBtn = new QPushButton("Выбрать цвет фона", this);
    backgroundColorBtn->setStyleSheet("border-radius: 20px; background-color: lightgray; padding: 10px;");
    layout->addWidget(backgroundColorBtn);

    // Кнопка для выбора текста
    QPushButton *textColorBtn = new QPushButton("Выбрать цвет текста", this);
    textColorBtn->setStyleSheet("border-radius: 20px; background-color: lightgray; padding: 10px;");
    layout->addWidget(textColorBtn);

    // Метка для отображения текста
    label = new QLabel("Пример текста", this);
    layout->addWidget(label);

    loadColors(); // Загружаем сохраненные цвета
    applyColors(); // Применяем начальные цвета

    connect(backgroundColorBtn, &QPushButton::clicked, this, &ColorPalette::chooseBackgroundColor);
    connect(textColorBtn, &QPushButton::clicked, this, &ColorPalette::chooseTextColor);
}

void ColorPalette::chooseBackgroundColor() {
    QColor color = QColorDialog::getColor(backgroundColor, this);
    if (color.isValid()) {
        backgroundColor = color;
        applyColors();
        saveColors();
        emit colorsChanged(backgroundColor, textColor); // Испускаем сигнал
    }
}

void ColorPalette::chooseTextColor() {
    QColor color = QColorDialog::getColor(textColor, this);
    if (color.isValid()) {
        textColor = color;
        applyColors();
        saveColors();
        emit colorsChanged(backgroundColor, textColor); // Испускаем сигнал
    }
}

void ColorPalette::applyColors() {
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, backgroundColor);
    this->setPalette(palette);
    //this->setAutoFillBackground(true);
    label->setStyleSheet(QString("color: %1").arg(textColor.name()));
}


void ColorPalette::saveColors() {
    QSettings settings("MyApp", "Settings");
    settings.setValue("backgroundColor", backgroundColor.name()); // Сохраняем QColor напрямую
    settings.setValue("textColor", textColor.name());
}

void ColorPalette::loadColors() {
    QSettings settings("MyApp", "Settings");
    QString bgColor = settings.value("backgroundColor", "#FFFFFF").toString();
    QString txtColor = settings.value("textColor", "#000000").toString();
    backgroundColor = QColor(bgColor);
    textColor = QColor(txtColor);
}

