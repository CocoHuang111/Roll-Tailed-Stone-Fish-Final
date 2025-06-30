#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

#include <QDateTime>
#include <QString>


class Announcement
{
public:
    Announcement(const QString& user, const QString& content, const QDateTime& time = QDateTime::currentDateTime());

    QString user() const;
    QString content() const;
    QDateTime time() const;

    QJsonObject toJson() const;
    static Announcement fromJson(const QJsonObject& json);

private:
    QString m_user;
    QString m_content;
    QDateTime m_time;
};

#endif // ANNOUNCEMENT_H
