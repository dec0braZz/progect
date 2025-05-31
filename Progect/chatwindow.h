#ifndef CHATWINDOW_H
#define CHATWINDOW_H


#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;

class ChatWindow : public QWidget {
    Q_OBJECT

public:
    explicit ChatWindow(const QString& friendName, QWidget* parent = nullptr);

public slots:
    void sendMessage();

private:
    QString friendName;
    QTextEdit* chatDisplay;
    QLineEdit* messageInput;
    QPushButton* sendButton;
};

#endif // CHATWINDOW_H
