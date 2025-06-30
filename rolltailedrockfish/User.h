#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <QString>
#include <QJsonObject>
#include <QCryptographicHash>

class Book; // 前向声明

class User {
public:
    //交易记录（选择性添加）
    struct TransactionRecord {
        QDateTime time;
        QString type;       // "purchase" 或 "recharge"
        double amount;
        QString detail;     // 如购买的书籍ISBN
    };

    std::string username;
    std::string password;
    std::unordered_set<std::string> preferences;
    std::vector<Book*> booksOnSale;
    std::string contact;
    double tokens;
    std::vector<std::string> achievements;
    std::vector<TransactionRecord> transactionHistory;

    User(const std::string& name, const std::string& pwd,
         const std::string& contactInfo, double initialTokens = 0.0);

    // 各种成员函数声明...
    QJsonObject toJson() const;
    static User* fromJson(const QJsonObject &obj);
    bool verifyPassword(const std::string& inputPassword) const;
    bool removeBookFromSale(Book* book);
    void addBookForSale(Book* book);
    const std::vector<Book*>& getBooksOnSale() const;
    void rechargeTokens(double amount);
    double getTokens() const;
    bool purchaseBook(Book* book);
    void addTransactionRecord(const TransactionRecord& record);
    const std::vector<TransactionRecord>& getTransactionHistory() const;
};

#endif // USER_H
