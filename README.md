# Roll-Tailed-Stone-Fish-Final
翻尾石鱼二手书交易平台
https://docs/images/logo.png <!-- 如果有logo的话 -->

一个基于Qt框架开发的校园二手书交易平台，提供书籍买卖、社区交流和用户私信等功能。

## 功能特性

# 核心功能

- 书籍上架与下架管理

- 多条件书籍搜索（标题、作者、标签等）
  
- 用户间实时聊天系统
  
- 社区公告发布与浏览
  
- 用户账户与交易记录管理

特色功能

- 智能标签分类系统
  
- 响应式UI设计
  
- JSON数据本地持久化
  
- 自适应布局

  
技术栈

- 编程语言: C++11

- GUI框架: Qt 5.15

- 数据持久化: JSON

- 构建系统: qmake

- 设计模式:

- MVC-like架构

- 工厂模式对象创建

- 观察者模式（信号槽）

项目结构

text
FlippingStoneFish/
├── docs/                    # 项目文档
├── include/                 # 头文件
│   ├── Announcement.h       # 公告类
│   ├── Book.h               # 书籍模型
│   ├── BookShelf.h          # 书籍管理
│   ├── Chat.h               # 聊天功能
│   ├── User.h               # 用户模型
│   └── UserManager.h        # 用户管理
├── src/                     # 源文件
│   ├── Announcement.cpp
│   ├── Book.cpp
│   ├── BookShelf.cpp
│   ├── Chat.cpp
│   ├── MainWindow.cpp       # 主界面
│   ├── main.cpp             # 程序入口
│   └── UserManager.cpp
├── ui/                      # UI设计文件
├── resources/               # 资源文件
│   ├── images/              # 图片资源
│   └── styles/              # 样式表
├── data/                    # 数据文件
│   ├── books.json           # 书籍数据
│   ├── users.json           # 用户数据
│   └── chats/               # 聊天记录
├── CMakeLists.txt           # CMake构建配置
└── README.md                # 本文件
快速开始

构建要求

Qt 5.15+ 开发环境
C++11兼容编译器
CMake 3.10+
构建步骤

克隆仓库：
bash
git clone https://github.com/yourrepo/FlippingStoneFish.git
cd FlippingStoneFish
使用CMake构建：
bash
mkdir build && cd build
cmake ..
make
运行程序：
bash
./FlippingStoneFish
开发环境配置

安装Qt Creator
导入项目文件夹
配置Qt Kit（建议使用Desktop Qt 5.15.x）
使用指南

用户流程

注册/登录：
新用户需提供用户名、密码和联系方式
已有用户直接登录
书籍交易：
卖家：通过"放生"页面上架书籍
买家：通过"钓书"页面搜索并联系卖家
社区互动：
在"捞书"页面查看和发布公告
通过"聊天墙"与交易方沟通
数据管理

所有数据自动保存在data/目录下
程序退出时会自动保存状态
手动备份建议复制整个data/文件夹
设计文档

架构设计

采用MVC-like架构分离：

模型：Book, User, Announcement等数据类
视图：MainWindow和各种Dialog界面
控制器：分散在UI类中的事件处理逻辑
核心类图

plantuml
@startuml
class Book {
  +title : QString
  +author : QString
  +toJson() : QJsonObject
  +fromJson() : Book*
}

class User {
  +username : std::string
  +password : std::string
  +contact : std::string
  +purchaseBook()
}

class MainWindow {
  -currentUser : User*
  +updateUserInfo()
  +refreshDisplay()
}

Book "1" -- "*" BookShelf
User "1" -- "*" Book
MainWindow --> Book
MainWindow --> User
@enduml
贡献指南

欢迎贡献！请遵循以下流程：

Fork本仓库
创建特性分支 (git checkout -b feature/your-feature)
提交更改 (git commit -am 'Add some feature')
推送到分支 (git push origin feature/your-feature)
创建Pull Request
许可证

本项目采用 MIT License

联系方式

如有任何问题，请联系：

项目维护者：yourname@example.com
问题追踪：https://github.com/yourrepo/FlippingStoneFish/issues
📅 最后更新：2023年10月
🛠️ 版本：1.0.0-beta
