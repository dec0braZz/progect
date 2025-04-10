#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <QWidget>
#include <QApplication>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QPushButton>
#include <QSettings>

class ColorPalette : public QWidget {
    Q_OBJECT

public:
    explicit ColorPalette(QWidget* parent = nullptr); // Конструктор с родителем
    ~ColorPalette() = default;

private slots:
    void chooseBackgroundColor();
    void chooseTextColor();

private:
    QColor backgroundColor = Qt::white;
    QColor textColor = Qt::black;
    QLabel *label;

    void applyColors();
    void saveColors();
    void loadColors();

signals:
    void colorsChanged(QColor background, QColor text);
};

#endif // COLORPALETTE_H
