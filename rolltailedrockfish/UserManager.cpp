#include "UserManager.h"
#include "User.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QFileInfo>
#include <QSaveFile>
#include <QDir>

UserManager::~UserManager() {
    saveUsersToFile();
    for (auto& pair : users) delete pair.second;
}

bool UserManager::createUser(const QString &username, const QString &password, const QString &contact) {
    // 1. 检查用户名是否已存在
    if (users.find(username) != users.end()) {
        qDebug() << "用户名已存在:" << username;
        return false;
    }

    // 2. 创建新用户（实际项目中密码应先哈希加密！）
    User *newUser = new User(
        username.toStdString(),
        password.toStdString(), // 注意：实际项目应存储密码的哈希值，而非明文！
        contact.toStdString()
        );

    // 3. 添加到用户列表
    users[username] = newUser;

    // 4. 自动保存到文件
    saveUsersToFile();

    return true;
}

User* UserManager::getUser(const QString& username) {
    auto it = users.find(username);
    return (it != users.end()) ? it->second : nullptr;
}

bool UserManager::saveUsersToFile() {
    QJsonArray jsonArray;

    // 确保目录存在
    QFileInfo fileInfo(dataFilePath);
    if (!QDir().mkpath(fileInfo.path())) {
        qWarning() << "无法创建目录:" << fileInfo.path();
        return false;
    }

    // 构建JSON数据
    for (const auto& [username, user] : users) {
        jsonArray.append(user->toJson());
    }

    // 原子写入（避免数据损坏）
    QSaveFile file(dataFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "文件打开失败:" << file.errorString();
        return false;
    }
    file.write(QJsonDocument(jsonArray).toJson());
    return file.commit(); // 只有成功才会替换原文件
}

bool UserManager::loadUsersFromFile() {
    QFile file(dataFilePath);
    if (!file.exists()) return true; // 文件不存在不算错误

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "文件读取失败:" << file.errorString();
        return false;
    }

    // 解析JSON
    QJsonArray jsonArray = QJsonDocument::fromJson(file.readAll()).array();
    for (const auto& userJson : jsonArray) {
        User* user = User::fromJson(userJson.toObject());
        if (user) {
            users[QString::fromStdString(user->username)] = user;
        }
    }
    return true;
}
