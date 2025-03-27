#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QObject>
#include <QLabel>//гифка
#include <QMovie>//гифка
#include <QTimer>
#include <QVBoxLayout>
#include <QMainWindow>
#include "secondwindow.h"
#include"okno.h"
class MainWindow : public QMainWindow {
public:
    MainWindow() {
        QLabel *imageLabel = new QLabel(this); // Создаем QLabel
        imageLabel->setStyleSheet("background: transparent;");
        imageLabel->setGeometry(300, 100, 900, 900); // Устанавливаем позицию и размер
        imageLabel->setPixmap(QPixmap("C:/Users/home/Desktop/qq/logo.png")); // Указываем путь к изображению
        imageLabel->setScaledContents(true); // Масштабируем содержимое
    Welcom();
    }
    void Welcom() {
        QLabel *WelcomGif = new QLabel(this);
        WelcomGif->setGeometry(530, 300, 200, 200); // Устанавливаем позицию для GIF
        // Установите фон для видимости
        WelcomGif->setAttribute(Qt::WA_TranslucentBackground);
        QMovie *movie = new QMovie("C:/Users/home/Desktop/qq/Welcome.gif");
        if (!movie->isValid()) {
            printf( "Ошибка загрузки GIF.");
        }

        WelcomGif->setMovie(movie);
        WelcomGif->show(); // Показываем QLabel с GIF
        movie->start(); // Запускаем анимацию
        WelcomGif->update(); // Обновляем QLabel
        connect(movie, &QMovie::finished, movie, &QMovie::start);
    }

};
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Создаем главное окно
    MainWindow *window = new MainWindow();
    window->setWindowTitle("KLOZ");
    window->setMinimumSize(1280, 860);
    window->setStyleSheet("background-image: url(C:/Users/home/Desktop/qq/fon.png); background-repeat: no-repeat;");


    SecondWindow *secWind = new SecondWindow;//память для регестрационного окна
    QPushButton Login("Login", window); // Указываем текст кнопки и родительский виджет
    Login.setGeometry(500, 650, 200, 50);
    Login.setStyleSheet("border-radius: 10px;"); // 10px - радиус скругления
    // Устанавливаем позицию и размеры кнопки
    QLabel *loginGif = new QLabel(window);
       QMovie *movie = new QMovie("C:/Users/home/Desktop/qq/loasing.gif"); // Путь GIF
       loginGif->setMovie(movie);
       loginGif->setGeometry(Login.geometry());
       loginGif->hide();
//анимация регист все тоже самое
    QPushButton Registr("Registr", window);
    Registr.setGeometry(540, 770, 120, 50);
    Registr.setStyleSheet("border-radius: 10px;");
    QLabel *registrGif = new QLabel(window);
        QMovie *registrMovie = new QMovie("C:/Users/home/Desktop/qq/loading.gif"); // Путь к вашему GIF
        registrGif->setMovie(registrMovie);
        registrGif->setGeometry(Registr.geometry());
        registrGif->hide();
        QObject::connect(&Login, &QPushButton::clicked, [&]() {
               Login.hide(); // Скрываем кнопку
               loginGif->show(); // Показываем анимацию
               movie->start(); // Запускаем анимацию
               // Открываем окно после завершения анимации
                   secWind->show();

                   });
    //Добавить кнопру Registr при регестрации сылку на сайт

    QObject::connect(&Registr, &QPushButton::clicked, [&]() {
           Registr.hide();
           registrGif->show();
           registrMovie->start();
    });
    // Автоматический возврат кнопки при закрытии окна
    QObject::connect(secWind, &SecondWindow::closed, [&]() {
        loginGif->hide();
        Login.show();
        registrGif->hide();
        Registr.show();
    });
    // Отображаем главное окно (show)
    window->show();
    //Выделение памяти
    OKNO *okno = new OKNO();
    okno->show();
    //доделать if когда будет гифка логин и регистр
    return app.exec();
}
