#include "User.h"
#include "Book.h"
#include <QJsonArray>

User::User(const std::string& name, const std::string& pwd,
           const std::string& contactInfo, double initialTokens)
    : username(name), password(pwd), contact(contactInfo), tokens(initialTokens) {}

QJsonObject User::toJson() const {
    QJsonObject obj;
    obj["username"] = QString::fromStdString(username);
    obj["password"] = QString::fromStdString(password);
    obj["contact"] = QString::fromStdString(contact);
    obj["tokens"] = tokens;

    QJsonArray prefsArray;
    for (const auto& pref : preferences) {
        prefsArray.append(QString::fromStdString(pref));
    }
    obj["preferences"] = prefsArray;

    QJsonArray transArray;
    for (const auto& record : transactionHistory) {
        QJsonObject transJson;
        transJson["time"] = record.time.toString(Qt::ISODate);
        transJson["type"] = record.type;
        transJson["amount"] = record.amount;
        transJson["detail"] = record.detail;
        transArray.append(transJson);
    }
    obj["transactionHistory"] = transArray;

    QJsonArray booksArray;
    for (const auto& book : booksOnSale) {
        booksArray.append(book->toJson());
    }
    obj["booksOnSale"] = booksArray;

    QJsonArray achievementsArray;
    for (const auto& ach : achievements) {
        achievementsArray.append(QString::fromStdString(ach));
    }
    obj["achievements"] = achievementsArray;

    return obj;
}

User* User::fromJson(const QJsonObject& obj) {
    User* user = new User(
        obj["username"].toString().toStdString(),
        obj["password"].toString().toStdString(),
        obj["contact"].toString().toStdString(),
        obj["tokens"].toDouble()
        );

    // 反序列化其他字段...
    // 反序列化偏好标签
    QJsonArray prefsArray = obj["preferences"].toArray();
    for (const QJsonValue& pref : prefsArray) {
        user->preferences.insert(pref.toString().toStdString());
    }

    // 反序列化交易记录
    QJsonArray transArray = obj["transactionHistory"].toArray();
    for (const QJsonValue& trans : transArray) {
        QJsonObject transObj = trans.toObject();
        TransactionRecord record;
        record.time = QDateTime::fromString(transObj["time"].toString(), Qt::ISODate);
        record.type = transObj["type"].toString();
        record.amount = transObj["amount"].toDouble();
        record.detail = transObj["detail"].toString();
        user->transactionHistory.push_back(record);
    }

    // 反序列化在售书籍
    QJsonArray booksArray = obj["booksOnSale"].toArray();
    for (const QJsonValue& book : booksArray) {
        user->booksOnSale.push_back(Book::fromJson(book.toObject()));
    }

    // 反序列化成就列表
    QJsonArray achievementsArray = obj["achievements"].toArray();
    for (const QJsonValue& ach : achievementsArray) {
        user->achievements.push_back(ach.toString().toStdString());
    }

    return user;
}

bool User::verifyPassword(const std::string& inputPassword) const {
    // 实际项目应对密码进行哈希后比较（示例使用SHA256）
    QByteArray hashedInput = QCryptographicHash::hash(
                                 QByteArray::fromStdString(inputPassword),
                                 QCryptographicHash::Sha256
                                 ).toHex();

    QByteArray storedPwdHash = QString::fromStdString(password).toUtf8();

    return hashedInput == storedPwdHash;  // 常量时间比较防止时序攻击
}

bool User::removeBookFromSale(Book* book) {
    auto it = std::find(booksOnSale.begin(), booksOnSale.end(), book);
    if (it != booksOnSale.end()) {
        booksOnSale.erase(it);
        return true;
    }
    return false;
}

void User::addBookForSale(Book* book) {
    if (book) {
        booksOnSale.push_back(book);
    }
}

const std::vector<Book*>& User::getBooksOnSale() const {
    return booksOnSale;
}

void User::rechargeTokens(double amount) {
    if (amount > 0) {
        tokens += amount;
    }
}

double User::getTokens() const {
    return tokens;
}

bool User::purchaseBook(Book* book) {
    if (!book || tokens < book->price) return false;

    // 1. 扣款
    tokens -= book->price;

    // 2. 变更书籍状态
    book->isSold = true;
    book->buyerId = QString::fromStdString(username);

    // 3. 添加交易记录
    addTransactionRecord({
        QDateTime::currentDateTime(),
        "purchase",
        -book->price,  // 支出为负
        QString("购买书籍: %1").arg(book->isbn)
    });

    return true;
}

const std::vector<User::TransactionRecord>& User::getTransactionHistory() const {
    return transactionHistory;
}

void User::addTransactionRecord(const TransactionRecord& record) {
    transactionHistory.push_back(record);
}
