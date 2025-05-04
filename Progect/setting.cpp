#include "setting.h"
#include "ColorPalette.h"
#include "socket.h"

Setting::Setting(QSharedPointer<QTcpSocket> sharedSocket, const QString& username, QWidget* parent)
    : QDialog(static_cast<QWidget*>(parent)), socket(sharedSocket), currentUsername(username) {
    setWindowTitle("Setting");

    setStyleSheet("background-color: grey;");
    resize(1280, 800);

    colorButton = new QPushButton("Открыть меню выбора цветов", this);
    colorButton->setGeometry(50, 100, 300, 50); // Положение и размер кнопки
    connect(colorButton, &QPushButton::clicked, this, &Setting::on_colorButton_clicked);

    colorPalette = nullptr; // Инициализация указателя на палитру

    loadSettingsFromServer();
}
//void Setting::applyColors(const QJsonObject &colorsObj) {
//    if (colorsObj.contains("backgroundColor") && colorsObj["backgroundColor"].isString()) {
//        this->setStyleSheet(QString("background-color: %1;").arg(colorsObj["backgroundColor"].toString()));
//    }

//    if (colorsObj.contains("textColor") && colorsObj["textColor"].isString()) {
//        this->setStyleSheet(QString("color: %1;").arg(colorsObj["textColor"].toString()));
//    }
//}
void Setting::applyColors(const QJsonObject &colorsObj) {
    QString style;

    if (colorsObj.contains("backgroundColor")) {
        QColor bgColor = QColor(colorsObj["backgroundColor"].toString());
        style += QString("background-color: %1; ").arg(bgColor.name());
    }

    if (colorsObj.contains("textColor")) {
        QColor txtColor = QColor(colorsObj["textColor"].toString());
        style += QString("color: %1; ").arg(txtColor.name());
    }

    if (!style.isEmpty())
        this->setStyleSheet(style);
}


void Setting::setCurrentUsername(const QString &username) {
    currentUsername = username;
    qDebug() << "Current username set to:" << currentUsername;
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
    sendSettingsToServer(currentUsername, background.name(), text.name()); //отправка на сервер
}
void Setting::sendSettingsToServer(const QString &username, const QString &bgColor, const QString &textColor) { //отправка на сервер
    // Формируем JSON
    QJsonObject json;
    json["command"] = "SAVE_SETTINGS";
    json["username"] = username;
    qDebug()<<username;
    json["backgroundColor"] = bgColor;
    json["textColor"] = textColor;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // Отправляем через QTcpSocket (например)
    socket->write(data);
     qDebug() << "Sending settings for user:" << username << "bgColor:" << bgColor << "textColor:" << textColor;
}
void Setting::loadSettingsFromServer() { //запросить данные с сервера
    // Запросить настройки
    QJsonObject json;
    json["command"] = "LOAD_SETTINGS";
    json["username"] = currentUsername;
    QJsonDocument doc(json);
    socket->write(doc.toJson());

    // При чтении данных с сервера парсить ответ и применять цвета
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
