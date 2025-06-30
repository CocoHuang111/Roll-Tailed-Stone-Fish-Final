#include "Announcement.h"
#include <QJsonObject>

Announcement::Announcement(const QString& user, const QString& content, const QDateTime& time)
    : m_user(user), m_content(content), m_time(time) {}

QString Announcement::user() const { return m_user; }

QString Announcement::content() const { return m_content; }

QDateTime Announcement::time() const { return m_time; }

QJsonObject Announcement::toJson() const {
    QJsonObject obj;
    obj["user"] = m_user;
    obj["content"] = m_content;
    obj["time"] = m_time.toString(Qt::ISODate);
    return obj;
}

Announcement Announcement::fromJson(const QJsonObject& json) {
    return Announcement(
        json["user"].toString(),
        json["content"].toString(),
        QDateTime::fromString(json["time"].toString(), Qt::ISODate)
        );
}

