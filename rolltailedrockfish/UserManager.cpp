#include "UserManager.h"
#include "User.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

UserManager::~UserManager() {
    saveUsersToFile();
    for (auto& pair : users) delete pair.second;
}

bool UserManager::createUser(const QString &username, const QString &password, const QString &contact, const QString &address) {
    // 1. 创建新用户（实际项目中密码应先哈希加密！）
    User *newUser = new User(
        username.toStdString(),
        password.toStdString(),  // 注意：实际项目应存储密码的哈希值，而非明文！
        contact.toStdString()
        );

    // 2. 添加到用户列表
    users[username] = newUser;

    // 3. 自动保存到文件
    saveUsersToFile();

    return true;
}

User* UserManager::getUser(const QString& username) {
    auto it = users.find(username);
    return (it != users.end()) ? it->second : nullptr;
}

bool UserManager::saveUsersToFile() {
    QJsonArray jsonArray;
    for (const auto& pair : users) {
        jsonArray.append(pair.second->toJson());
    }
    QFile file(dataFilePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(jsonArray).toJson());
    return true;
}

bool UserManager::loadUsersFromFile() {
    QFile file(dataFilePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    QByteArray data = file.readAll();
    QJsonArray jsonArray = QJsonDocument::fromJson(data).array();

    for (const auto& jsonValue : jsonArray) {
        User* user = User::fromJson(jsonValue.toObject());
        users[QString::fromStdString(user->username)] = user;
    }
    return true;
}
