#include "chatwindow.h"
#include "ChatWindow.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

ChatWindow::ChatWindow(const QString& friendName, QWidget* parent)
    : QWidget(parent), friendName(friendName)
{
    setWindowTitle("Чат с " + friendName);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Отправить", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
}

void ChatWindow::sendMessage() {
    QString message = messageInput->text().trimmed();
    if (!message.isEmpty()) {
        chatDisplay->append("Вы: " + message);
        messageInput->clear();

        // Добавьте здесь логику отправки сообщения собеседнику
    }
}
