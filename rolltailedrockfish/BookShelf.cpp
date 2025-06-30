#include "BookShelf.h"
#include <QJsonArray>
#include <algorithm>
#include <QMessageBox>

BookShelf::BookShelf()
{
    loadFromFile();
}

bool BookShelf::loadFromFile(const QString &path) {
    QString filePath = path.isEmpty() ? m_storagePath : path;
    if (filePath.isEmpty()) {
        qWarning() << "No storage path specified";
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return false;
    }

    QJsonArray jsonArray = QJsonDocument::fromJson(file.readAll()).array();
    for (const QJsonValue &value : jsonArray) {
        Book* book = Book::fromJson(value.toObject());
        if(book) books.push_back(book);
    }

    return true;
}

bool BookShelf::saveToFile(const QString &path) const {
    QString filePath = path.isEmpty() ? m_storagePath : path;
    if (filePath.isEmpty()) {
        qWarning() << "No storage path specified";
        return false;
    }

    // 准备要保存的JSON数据
    QJsonArray jsonArray;
    for (const Book* book : books) {
        jsonArray.append(book->toJson());
    }
    QByteArray jsonData = QJsonDocument(jsonArray).toJson();

    // 直接打开原文件写入（覆盖模式）
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    // 写入数据
    if (file.write(jsonData) == -1) {
        qWarning() << "Failed to write file:" << file.errorString();
        file.close();
        return false;
    }

    file.close();
    return true;


}

bool BookShelf::validateBook(const Book* book) const {
    // 实现你的验证逻辑
    return !book->title.isEmpty();
    // 可以根据需要添加更多验证条件
}

bool BookShelf::isbnExists(const QString &isbn) const {
    return std::any_of(books.begin(), books.end(),
                       [&isbn](const Book* b) { return b->isbn == isbn; });
}

bool BookShelf::addBook(const Book* book) {
    if (!validateBook(book)) {
        qWarning() << "Invalid book data";
        return false;
    }
    /*
    QMessageBox::information(nullptr, "调试信息", "运行至此处");
    if (isbnExists(book->isbn)) {
        qWarning() << "Book with ISBN" << book->isbn << "already exists";
        return false;
    }
    */
    books.append(new Book(*book));
    saveToFile();
    return true;
}

bool BookShelf::removeBook(const QString&isbn) {
    auto it = std::find_if(books.begin(), books.end(),
                           [&isbn](const Book* b) { return b->isbn == isbn; });

    if (it != books.end()) {
        delete *it;
        books.erase(it);
        return true;
    }
    return false;
}

bool BookShelf::updateBook(const Book* book) {
    if (!validateBook(book)) return false;

    for (auto &b : books) {
        if (b->isbn == book->isbn) {
            *b = *book;
            return true;
        }
    }
    return false;
}

Book* BookShelf::findBook(const QString& isbn, std::vector<Book*> books){
    Book* ans = nullptr;
    for(Book* book : books){
        if(book->isbn == isbn){
            ans = book;
            break;
        }
    }
    return ans;
}
