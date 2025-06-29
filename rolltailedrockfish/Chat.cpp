#include "Chat.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <algorithm>

ChatMessage::ChatMessage(const QString& sender, const QString& content, const QDateTime& time)
    : sender(sender), content(content), timestamp(time) {}

QJsonObject ChatMessage::toJson() const {
    QJsonObject obj;
    obj["sender"] = sender;
    obj["content"] = content;
    obj["time"] = timestamp.toString(Qt::ISODate);
    return obj;
}

ChatMessage ChatMessage::fromJson(const QJsonObject& json) {
    return ChatMessage(
        json["sender"].toString(),
        json["content"].toString(),
        QDateTime::fromString(json["time"].toString(), Qt::ISODate)
        );
}

Chat::Chat(const QString& user1, const QString& user2)
    : user1(user1), user2(user2) {}

void Chat::addMessage(const QString& sender, const QString& content) {
    messages.append(ChatMessage(sender, content));

    QDir().mkdir("chats");

    QString filename = "chats/" + getChatFileName(user1, user2);
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(toJson());
        file.write(doc.toJson());
        file.close();
    } else qDebug() << "无法保存聊天记录到文件:" << file.errorString();
}

QJsonObject Chat::toJson() const {
    QJsonObject obj;
    obj["user1"] = user1;
    obj["user2"] = user2;

    QJsonArray msgArray;
    for (const auto& msg : messages) {
        msgArray.append(msg.toJson());
    }
    obj["messages"] = msgArray;

    return obj;
}

Chat Chat::fromJson(const QJsonObject& json) {
    Chat chat(json["user1"].toString(), json["user2"].toString());

    QJsonArray msgArray = json["messages"].toArray();
    for (const QJsonValue& val : msgArray) {
        chat.messages.append(ChatMessage::fromJson(val.toObject()));
    }

    return chat;
}

QString Chat::getChatFileName(const QString& user1, const QString& user2) {
    QStringList users = {user1, user2};
    std::sort(users.begin(), users.end());
    return QString("chat_%1_%2.json").arg(users[0], users[1]);
}

Chat Chat::loadChat(const QString& user1, const QString& user2) {
    QString filename = "chats/" + getChatFileName(user1, user2);
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        return fromJson(doc.object());
    }

    // 文件不存在时返回新Chat对象
    return Chat(user1, user2);
}

