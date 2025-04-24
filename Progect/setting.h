#ifndef SETTING_H
#define SETTING_H
#include <QWidget>
#include <QDialog>
#include <QCloseEvent>
#include <QApplication>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QSettings>
#include "ColorPalette.h"
#include  <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>
class Setting : public QDialog{
    Q_OBJECT
public:
     Setting(QSharedPointer<QTcpSocket> sharedSocket, const QString& username, QWidget* parent = nullptr);
    void toggleColorPalette();
    void openSetting();
    void on_colorButton_clicked();
private slots:
    void openColorPalette();
    void closeColorPalette();
    void updateColors(const QColor &background, const QColor &text);
private:
     QPushButton *colorButton;
     ColorPalette* colorPalette;
     QString currentUsername;

     QSharedPointer<QTcpSocket> socket;
public slots:
     void loadSettingsFromServer();
protected:
     void sendSettingsToServer(const QString &username, const QString &bgColor, const QString &textColor);
     void setCurrentUsername(const QString &username);
signals:
    void colorsChanged(const QColor &background, const QColor &text);
    void closed();
};

#endif // SETTING_H
