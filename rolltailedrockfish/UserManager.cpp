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
    // 1. 准备JSON数据
    QJsonArray jsonArray;
    for (const auto& [username, user] : users) {
        jsonArray.append(user->toJson());
    }

    // 2. 确保目录存在（使用绝对路径）
    QFileInfo fileInfo(dataFilePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists() && !dir.mkpath(".")) {
        qWarning() << "无法创建目录:" << dir.absolutePath();
        return false;
    }

    // 3. 使用QSaveFile进行原子写入
    QSaveFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "文件打开失败:" << file.errorString()
            << "路径:" << fileInfo.absoluteFilePath();
        return false;
    }

    // 4. 写入数据
    QJsonDocument doc(jsonArray);
    qint64 bytesWritten = file.write(doc.toJson(QJsonDocument::Indented));
    if (bytesWritten == -1) {
        qWarning() << "写入文件失败:" << file.errorString();
        return false;
    }

    // 5. 提交更改（只有成功才会替换原文件）
    if (!file.commit()) {
        qWarning() << "文件提交失败:" << file.errorString();
        return false;
    }

    qDebug() << "用户数据成功保存到:" << fileInfo.absoluteFilePath();
    return true;
}

bool UserManager::loadUsersFromFile() {
    QFile file(dataFilePath);
    if (!file.exists()) {
        qDebug() << "用户数据文件不存在，将创建新文件";
        return true; // 文件不存在不是错误，首次运行时会创建
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开用户数据文件:" << file.errorString();
        return false;
    }

    // 读取并解析JSON
    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qWarning() << "用户数据文件格式错误";
        return false;
    }

    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        User* user = User::fromJson(value.toObject());
        if (user) {
            users[QString::fromStdString(user->username)] = user;
        }
    }

    qDebug() << "成功加载" << users.size() << "个用户";
    return true;
}
