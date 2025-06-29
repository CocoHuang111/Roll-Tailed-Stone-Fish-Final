#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QDateTime>
#include <QJsonObject>
#include <QList>

class ChatMessage {
public:
    QString sender;
    QString content;
    QDateTime timestamp;

    ChatMessage(const QString& sender, const QString& content, const QDateTime& time = QDateTime::currentDateTime());
    QJsonObject toJson() const;
    static ChatMessage fromJson(const QJsonObject& json);
};

class Chat
{
public:
    QString user1;
    QString user2;
    QList<ChatMessage> messages;

    Chat(const QString& user1, const QString& user2);

    void addMessage(const QString& sender, const QString& content);
    QJsonObject toJson() const;
    static Chat fromJson(const QJsonObject& json);
    static QString getChatFileName(const QString& user1, const QString& user2);
    Chat loadChat(const QString& user1, const QString& user2);
};

#endif // CHAT_H
