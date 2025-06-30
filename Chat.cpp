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

Chat::Chat(const QString& user1, const QString& user2): user1(user1), user2(user2) {
    QString filename = "chats/" + getChatFileName(user1, user2);
    QFile file(filename);

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject json = doc.object();

        // 反序列化已有消息
        QJsonArray msgArray = json["messages"].toArray();
        for (const QJsonValue& val : msgArray) {
            messages.append(ChatMessage::fromJson(val.toObject()));
        }
        file.close();
    }
}

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

QList<QString> Chat::getContacts(const QString& currentUser) {
    QList<QString> contacts;
    QDir dir("chats");

    // 获取所有聊天记录文件
    QStringList filters;
    filters << "chat_*.json";
    dir.setNameFilters(filters);

    foreach (QFileInfo fileInfo, dir.entryInfoList()) {
        QString filename = fileInfo.fileName();

        // 解析文件名格式 chat_user1_user2.json
        QStringList parts = filename.mid(5, filename.length()-10).split('_');
        if (parts.size() == 2) {
            QString user1 = parts[0];
            QString user2 = parts[1];

            if (user1 == currentUser) {
                if (!contacts.contains(user2)) contacts.append(user2);
            } else if (user2 == currentUser) {
                if (!contacts.contains(user1)) contacts.append(user1);
            }
        }
    }

    return contacts;
}
