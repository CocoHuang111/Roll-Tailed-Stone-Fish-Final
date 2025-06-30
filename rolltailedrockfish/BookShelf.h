#ifndef BOOKSHELF_H
#define BOOKSHELF_H

#include "Book.h"
#include <QList>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

class BookShelf {
private:
    QString m_storagePath = "books.json"; // 本地存储路径
    bool validateBook(const Book* book) const;
    bool isbnExists(const QString& isbn) const;

public:
    QList<Book*> books;  // 内存存储
    BookShelf();
    // 增删改查
    bool addBook(const Book* book);
    bool removeBook(const QString& isbn);
    bool updateBook(const Book* book);
    Book* findBook(const QString& isbn, QList<Book*> books);

    // 批量操作
    //QList<Book> findByAuthor(const QString& author) const;
    //QList<Book> findByTag(const QString& tag) const;

    // 持久化
    bool loadFromFile(const QString& path = "");
    bool saveToFile(const QString& path = "") const;

    // 统计
    int count() const { return books.size(); }
};



#endif // BOOKSHELF_H
