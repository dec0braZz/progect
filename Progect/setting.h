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
class Setting : public QDialog{
    Q_OBJECT
public:

    explicit Setting (QWidget *parent = nullptr);
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
protected:
signals:
    void colorsChanged(const QColor &background, const QColor &text);
    void closed();
};

#endif // SETTING_H
