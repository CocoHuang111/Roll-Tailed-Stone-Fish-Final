#include "Book.h"
#include <QJsonArray>

bool Book::operator==(const Book &other) const {
    return isbn == other.isbn;
}

Book::Book(const Book* other) :
    title(other->title),
    author(other->author),
    publisher(other->publisher),
    publishYear(other->publishYear),
    isbn(other->isbn),
    tags(other->tags), // QStringList 有隐式共享，无需深拷贝
    price(other->price),
    description(other->description),
    coverImage(other->coverImage),
    listedTime(other->listedTime),
    sellerId(other->sellerId),
    isSold(other->isSold),
    buyerId(other->buyerId)
{}

QJsonObject Book::toJson() const {
    QJsonObject obj;

    // 基础信息
    obj["title"] = title;
    obj["author"] = author;
    obj["publisher"] = publisher;
    obj["publishYear"] = publishYear;
    obj["isbn"] = isbn;
    obj["price"] = price;
    obj["description"] = description;
    obj["coverImage"] = coverImage;
    obj["sellerId"] = sellerId;
    obj["buyerId"] = buyerId;
    obj["isSold"] = isSold;

    // 特殊类型处理
    obj["listedTime"] = listedTime.toString(Qt::ISODate);

    QJsonArray tagsArray;
    for (const QString& tag : tags) {
        tagsArray.append(tag);
    }
    obj["tags"] = tagsArray;

    return obj;
}

Book* Book::fromJson(const QJsonObject &obj) {
    if (obj.isEmpty()) return nullptr;

    try {
        // 必需字段检查
        if (!obj.contains("title") || !obj.contains("isbn")) {
            throw std::invalid_argument("Missing required fields");
        }

        // 构造对象
        Book* book = new Book();

        // 基础字段
        book->title = obj["title"].toString();
        book->author = obj["author"].toString();
        book->publisher = obj["publisher"].toString();
        book->publishYear = obj["publishYear"].toInt();
        book->isbn = obj["isbn"].toString();
        book->price = obj["price"].toDouble();
        book->description = obj["description"].toString();
        book->coverImage = obj["coverImage"].toString();
        book->sellerId = obj["sellerId"].toString();
        book->buyerId = obj["buyerId"].toString();
        book->isSold = obj["isSold"].toBool();

        // 特殊类型处理
        book->listedTime = QDateTime::fromString(obj["listedTime"].toString(), Qt::ISODate);

        // 反序列化标签列表
        if (obj.contains("tags")) {
            QJsonArray tagsArray = obj["tags"].toArray();
            for (const QJsonValue& tag : tagsArray) {
                book->tags.append(tag.toString());
            }
        }
        return book;
    } catch (const std::exception& e) {
        qWarning() << "Book反序列化失败:" << e.what();
        return nullptr;
    }
}

bool Book::saveToFile(const QList<Book>& books, const QString& filePath) {
    // 1. 准备JSON数组
    QJsonArray jsonArray;
    for (const Book& book : books) {
        jsonArray.append(book.toJson());
    }

    // 2. 创建JSON文档
    QJsonDocument doc(jsonArray);

    // 3. 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法打开文件:" << file.errorString();
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented)); // Indented格式更易读
    file.close();
    return true;
}

QList<Book> Book::loadFromFile(const QString& filePath) {
    QList<Book> books;

    // 1. 打开文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return books; // 返回空列表
    }

    // 2. 读取并解析JSON
    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isArray()) {
        qWarning() << "Invalid JSON format or not an array";
        return books;
    }

    // 3. 转换为Book对象
    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        QJsonObject obj = value.toObject();
        books.append(*Book::fromJson(obj)); // 复用现有的fromJson方法
    }

    return books;
}
/*
使用示例：
// 保存示例
QList<Book> allBooks;
allBooks.append(book1);
allBooks.append(book2);

if (Book::saveToFile(allBooks)) {
    qDebug() << "保存成功";
}

// 加载示例
QList<Book> loadedBooks = Book::loadFromFile();
if (!loadedBooks.isEmpty()) {
    qDebug() << "共加载" << loadedBooks.size() << "本书";
}

// 加载书籍数据
QList<Book> allBooks = Book::loadFromFile();
if (allBooks.isEmpty()) {
    qDebug() << "没有加载到书籍数据，可能是文件不存在或为空";
} else {
    qDebug() << "成功加载" << allBooks.size() << "本书";
    for (const Book& book : allBooks) {
        // 处理每本书...
    }
}*/
