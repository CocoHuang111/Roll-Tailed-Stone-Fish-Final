#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QJsonDocument>

class Book {
public:
    QString title;
    QString author;
    QString publisher;
    int publishYear;
    QString isbn;
    QStringList tags;
    double price;
    QString description;
    QString coverImage;
    QDateTime listedTime;
    QString sellerId;
    bool isSold;
    QString buyerId;    // 购买者ID

    Book(
        const QString& title = "",
        const QString& author = "",
        const QString& publisher = "",
        int publishYear = 0,
        const QString& isbn = "",
        const QStringList& tags = QStringList(),
        double price = 0.0,
        const QString& description = "",
        const QString& coverImage = "",
        const QDateTime& listedTime = QDateTime::currentDateTime(),
        const QString& sellerId = "",
        bool isSold = false,
        const QString& buyerId = ""
        ) :
        title(title),
        author(author),
        publisher(publisher),
        publishYear(publishYear),
        isbn(isbn),
        tags(tags),
        price(price),
        description(description),
        coverImage(coverImage),
        listedTime(listedTime),
        sellerId(sellerId),
        isSold(isSold),
        buyerId(buyerId)
    {}
    
    Book(const Book* other);
    // 辅助函数
    bool operator==(const Book &other) const;
    QJsonObject toJson() const;
    static Book* fromJson(const QJsonObject &obj);
    //6.25 新增保存函数
    static bool saveToFile(const QList<Book>& books, const QString& filePath = "books.json");
    static QList<Book> loadFromFile(const QString& filePath = "books.json");
};

#endif // BOOK_H
