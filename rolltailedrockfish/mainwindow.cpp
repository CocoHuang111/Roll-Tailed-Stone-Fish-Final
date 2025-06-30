#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QSpacerItem>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QScrollArea>
#include <QTextBrowser>
#include <QMessageBox>
#include "registerdialog.h"
#include "findbookdialog.h"
#include "Book.h"
#include "User.h"
#include "UserManager.h"
#include "announcementdialog.h"
#include "Chat.h"
#include "displaydialog.h"
#include "mybookdialog.h"

User* currentUser = nullptr;
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , bs()
    ,background(":/image/background.jpg")
    ,networkManager(new QNetworkAccessManager(this))
    ,current_page(0)
{
    ui->setupUi(this);
    setAutoFillBackground(false);
    if (!qy.initialize()) {
        qCritical() << "无法初始化用户管理器";
        // 处理错误情况
    }

    //左侧菜单栏
    QVBoxLayout *leftLayout=new QVBoxLayout;
    btn1 = new ResizeButton("登陆/注册");
    btn2 = new ResizeButton("我的主页");
    btn3 = new ResizeButton("钓  书");
    btn4 = new ResizeButton("捞  书");
    btn5 = new ResizeButton("放  生");
    btn6 = new ResizeButton("聊天墙");
    btn7 = new ResizeButton("成就墙");
    QWidget *menu = new QWidget;
    QList<ResizeButton*> buttons = {btn1, btn2, btn3, btn4, btn5, btn6, btn7};
    leftLayout->setSpacing(20);
    leftLayout->setContentsMargins(30,30,30,30);
    for (auto btn : buttons) {
        btn->setStyleSheet(
            "QPushButton{"
            "background-color: rgba(73, 150, 255, 158);"
            "color:white;"
            "border:3px solid white;"
            "border-radius:20px;"
            " padding: 8px;"
            "}"
            "QPushButton:hover{"
            "background-color: rgb(120, 170, 255);"
            "}"
            );
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        leftLayout->addWidget(btn,1);
    }
    menu->setLayout(leftLayout);
    menu->setAttribute(Qt::WA_StyledBackground, true);
    menu->setStyleSheet("background-color: rgba(255, 255, 255, 0);");

    //右侧主区域
    //QStackedWidget *MainArea=new QStackedWidget;
    QWidget *page1=new QWidget;
    setpage1(page1);
    QWidget *page2=new QWidget;
    setpage2(page2);
    QWidget *page3=new QWidget;
    setpage3(page3);
    QWidget *page4=new QWidget;
    setpage4(page4);
    QWidget *page5=new QWidget;
    setpage5(page5);
    QWidget *page6=new QWidget;
    setpage6(page6);
    QWidget *page7=new QWidget;
    QList<QWidget*> pages={page1,page2,page3,page4,page5,page6,page7};
    for (auto page : pages){
        MainArea->addWidget(page);
    }
    MainArea->setCurrentIndex(0);
    btn1->setStyleSheet(
        "QPushButton{"
        "background-color: rgb(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "}"
        );

    //连接槽
    for(int i=0;i<7;i++){
        connect(buttons[i],&QPushButton::clicked,[=](){
            MainArea->setCurrentIndex(i);
            buttons[i]->setStyleSheet(
                "QPushButton{"
                "background-color: rgb(120, 170, 255);"
                "color:white;"
                "border:3px solid white;"
                "border-radius:20px;"
                " padding: 8px;"
                "}"
                );
            buttons[current_page]->setStyleSheet(
                "QPushButton{"
                "background-color: rgba(73, 150, 255, 158);"
                "color:white;"
                "border:3px solid white;"
                "border-radius:20px;"
                " padding: 8px;"
                "}"
                "QPushButton:hover{"
                "background-color: rgb(120, 170, 255);"
                "}"
                );
            current_page=i;
            if (i == 5) setpage6(page6);
        });
    }

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(menu, 1);
    mainLayout->addWidget(MainArea, 5);
    this->setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(this->rect(), background);
}

void MainWindow::setpage1(QWidget* pg){
    QLabel *l_logo=new QLabel;
    l_logo->setPixmap(QPixmap(":/image/logo.png"));
    l_logo->setMaximumSize(800, 500);
    //l_logo->setMaximumHeight(500);
    l_logo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    l_logo->setScaledContents(true);
    l_logo->setAlignment(Qt::AlignCenter);

    QLineEdit *nameInput = new QLineEdit;
    nameInput->setPlaceholderText("请输入姓名: ");
    nameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QString name = nameInput->text();
    //输入姓名 TODO
    QLineEdit *pwdInput = new QLineEdit;
    pwdInput->setEchoMode(QLineEdit::Password);
    pwdInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pwdInput->setPlaceholderText("请输入密码: ");
    //输入密码 TODO
    QString password = pwdInput->text();

    QHBoxLayout *loginbtns=new QHBoxLayout;
    loginbtns->setContentsMargins(150, 0,150, 0);
    loginbtns->setSpacing(100);
    ResizeButton *startnew=new ResizeButton("注册");
    startnew->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ResizeButton *login=new ResizeButton("确认登录");
    login->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    loginbtns->addWidget(startnew,1);
    loginbtns->addWidget(login,1);
    
    connect(startnew,&QPushButton::clicked,[=](){
        RegisterDialog *reg_diag = new RegisterDialog(this);
        reg_diag->show();
        bool flag = true;
        if (reg_diag->exec() == QDialog::Accepted) {
            QString username = reg_diag->getUsername();
            QString password = reg_diag->getPassword();
            QString contact = reg_diag->getContact();

            if (qy.createUser(username, password, contact)) {
                QMessageBox::information(this, "成功", "注册成功！");
            } else {
                QMessageBox::critical(this, "错误", "用户名已存在！");
            }
        }
    });
    
        connect(login,&QPushButton::clicked,[=](){
        QString curname = nameInput->text();
        QString curpassword = pwdInput->text();
        // 1. 输入验证
        if (curname.isEmpty() || curpassword.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "用户名和密码不能为空");
            return;
        }

        // 2. 调用UserManager验证登录
        User* user = qy.getUser(curname);

        // 3. 验证用户是否存在及密码是否正确
        if (!user || !user->verifyPassword(curpassword.toStdString())) {
            QMessageBox::critical(this, "登录失败", "用户名或密码错误");

            // 安全建议：清空密码框
            pwdInput->clear();

            return;
        }

        // 4. 登录成功处理
        QMessageBox::information(this, "成功", "登录成功！");
        currentUser = user;
        updateUserInfo();  // 立即刷新
        refreshOwnerbook(book_display);
        MainArea->setCurrentIndex(1);
                btn2->setStyleSheet(
            "QPushButton{"
            "background-color: rgb(120, 170, 255);"
            "color:white;"
            "border:3px solid white;"
            "border-radius:20px;"
            " padding: 8px;"
            "}"
            );
        btn1->setStyleSheet(
            "QPushButton{"
            "background-color: rgba(73, 150, 255, 158);"
            "color:white;"
            "border:3px solid white;"
            "border-radius:20px;"
            " padding: 8px;"
            "}"
            "QPushButton:hover{"
            "background-color: rgb(120, 170, 255);"
            "}"
            );
        current_page=1;
    });

    QVBoxLayout *fill_in=new QVBoxLayout;
    fill_in->setContentsMargins(200, 0, 200, 60);
    fill_in->addWidget(nameInput);
    fill_in->addStretch();
    fill_in->addWidget(pwdInput);
    fill_in->addStretch();
    fill_in->addLayout(loginbtns);

    QVBoxLayout *layout1=new QVBoxLayout;
    layout1->setContentsMargins(60, 40, 60, 40);
    layout1->addWidget(l_logo, 1, Qt::AlignCenter);
    layout1->addLayout(fill_in,1);

    pg->setLayout(layout1);
}

void MainWindow::updateUserInfo() {
    if(usernameLabel) {
        QString text = "用户名：";
        if(currentUser) {
            text += QString::fromStdString(currentUser->username);
            qDebug() << "更新用户名显示：" << text;
        }
        usernameLabel->setText(text);

        QString contactText = "联系方式：";
        if(currentUser) {
            contactText += QString::fromStdString(currentUser->contact);
        }
        contactLabel->setText(contactText);

        QString bookOnSale = "在售书籍：";
        if(currentUser) {
            for (const Book* book : bs.books) {
                if (!book->isSold && book->sellerId.toStdString() == currentUser->username) { // 只显示未售出的
                    bookOnSale += book->title;
                    bookOnSale += " ";
                }
            }
        }
        onsalebookLabel->setText(bookOnSale);
    }
}

void MainWindow::setpage2(QWidget* pg){
    QLabel *title=new QLabel("个人信息");
    title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
    );
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    usernameLabel = new QLabel("用户名：", pg); // 初始化
    usernameLabel->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );

    contactLabel = new QLabel("联系方式：",pg);
    contactLabel->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );

    onsalebookLabel = new QLabel("在售书籍",pg);
    onsalebookLabel->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );

    QLabel *record=new QLabel("交易记录：");
    record->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );

    QLabel *award=new QLabel("解锁成就：");
    award->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );

    QVBoxLayout *layout2=new QVBoxLayout;
    layout2->setContentsMargins(40,0,40,35);
    layout2->setSpacing(20);
    layout2->addWidget(title,2);
    //layout2->addStretch();
    layout2->addWidget(usernameLabel,1);
    //layout2->addStretch();
    layout2->addWidget(contactLabel,1);
    //layout2->addStretch();
    layout2->addWidget(onsalebookLabel,1);
    //layout2->addStretch();
    layout2->addWidget(record,1);
    //layout2->addStretch();
    layout2->addWidget(award,1);
    layout2->addStretch();

    pg->setLayout(layout2);
}

QList<Book> MainWindow::parseSearchResults(const QByteArray &jsonData) {
    QList<Book> books;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isArray()) {
        for (const QJsonValue &val : doc.array()) {
            QJsonObject obj = val.toObject();
            QString isbn = obj["isbn"].toString();
            books.append(bs.findBook(isbn, bs.books));
        }
    }
    return books;
}

// TODO 对display进行修改
void MainWindow::displaySearchResults(const QList<Book> &books, QWidget *container) {
    // 清空容器
    QLayout *layout = container->layout();
    layout->setSpacing(15);
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0))) {
            delete item->widget();
            delete item;
        }
    } else {
        layout = new QVBoxLayout(container);
    }

    // 动态创建结果项
    for (const Book &book : books) {
        QPushButton *bookbtn=new QPushButton(book.title);
        bookbtn->setFixedHeight(50);
        bookbtn->setStyleSheet(
            "QPushButton{"
            "background-color: rgba(73, 150, 255, 158);"
            "color:white;"
            "border:3px solid white;"
            "border-radius:20px;"
            " padding: 8px;"
            "}"
            "QPushButton:hover{"
            "background-color: rgb(120, 170, 255);"
            "}"
            );
        connect(bookbtn,&QPushButton::clicked,[=](){
            DisplayDialog *display=new DisplayDialog(const_cast<Book*>(&book),this);
            display->show();
            if(display->exec()==QDialog::Accepted){
                MainArea->setCurrentIndex(5);
            }
            delete display;
        });
        layout->addWidget(bookbtn);
    }
    container->setLayout(layout);
}

void MainWindow::setpage3(QWidget* pg){
    QLineEdit *search_column=new QLineEdit();
    search_column->setPlaceholderText("搜索： ");
    search_column->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QString search_content=search_column->text();
    ResizeButton *search_confirm=new ResizeButton("确定");
    search_confirm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *search_layout=new QHBoxLayout;
    search_layout->setContentsMargins(5,0,10,10);
    search_layout->addWidget(search_column,15);
    search_layout->setSpacing(10);
    search_layout->addWidget(search_confirm,1);

    QLabel *tag_title=new QLabel("选择tag：");
    tag_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tag_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );

    QHBoxLayout *tagsline1=new QHBoxLayout;
    tagsline1->setContentsMargins(20,0,10,0);
    ResizeButton *tag1=new ResizeButton("数学");
    ResizeButton *tag2=new ResizeButton("计算机");
    ResizeButton *tag3=new ResizeButton("中文");
    ResizeButton *tag4=new ResizeButton("物理");
    ResizeButton *tag5=new ResizeButton("化学");
    ResizeButton *tag6=new ResizeButton("生物");
    ResizeButton *tag7=new ResizeButton("政治");
    ResizeButton *tag8=new ResizeButton("英语");
    ResizeButton *tag9=new ResizeButton("经管");
    QList<ResizeButton*> alltag={tag1,tag2,tag3,tag4,tag5,tag6,tag7,tag8,tag9};
    for(auto tag :alltag){
        tag->setStyleSheet(
            "QPushButton{"
            "background-color: rgba(73, 150, 255, 158);"
            "color:white;"
            "border:3px solid white;"
            "border-radius:20px;"
            " padding: 8px;"
            "font-size:20px;"
            "}"
            "QPushButton:hover{"
            "background-color: rgb(120, 170, 255);"
            "font-size:20px;"
            "}"
            );
        tagsline1->addWidget(tag);
    }

    QHBoxLayout *tagsline2=new QHBoxLayout;
    tagsline1->setContentsMargins(20,0,10,0);
    ResizeButton *tag2_1=new ResizeButton("教材");
    ResizeButton *tag2_2=new ResizeButton("习题");
    ResizeButton *tag2_3=new ResizeButton("研究");
    ResizeButton *tag2_4=new ResizeButton("传记");
    ResizeButton *tag2_5=new ResizeButton("文学");
    QList<ResizeButton*> alltag2={tag2_1,tag2_2,tag2_3,tag2_4,tag2_5};
    for(auto tag :alltag2){
        tag->setStyleSheet(
            "QPushButton{"
            "background-color: rgba(73, 150, 255, 158);"
            "color:white;"
            "border:3px solid white;"
            "border-radius:20px;"
            " padding: 8px;"
            "font-size:20px;"
            "}"
            "QPushButton:hover{"
            "background-color: rgb(120, 170, 255);"
            "font-size:20px;"
            "}"
            );
        tagsline2->addWidget(tag);
    }

    auto setupTagButton = [this](ResizeButton* button) {
        connect(button, &QPushButton::clicked, [this, button]() {
            QString tag = button->text();
            if (selectedTags.contains(tag)) {
                selectedTags.remove(tag);
                button->setStyleSheet(normalStyle); // 恢复默认样式
            } else {
                selectedTags.insert(tag);
                button->setStyleSheet(selectedStyle); // 设置选中样式
            }
        });
    };

    // 应用到所有标签按钮
    for (auto tag : alltag) {
        tag->setStyleSheet(normalStyle);
        setupTagButton(tag);
    }
    for (auto tag : alltag2) {
        tag->setStyleSheet(normalStyle);
        setupTagButton(tag);
    }


    QLabel *guess_title=new QLabel("猜你喜欢：");
    guess_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    guess_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );
    //补充所有tag和猜你喜欢 TODO

    QVBoxLayout *recommend_layout=new QVBoxLayout;
    recommend_layout->addWidget(tag_title,2);
    recommend_layout->addLayout(tagsline1,1);
    recommend_layout->addStretch();
    recommend_layout->addLayout(tagsline2,1);
    recommend_layout->addWidget(guess_title,2);
    recommend_layout->addStretch();

    QWidget *recommend_page=new QWidget;
    recommend_page->setLayout(recommend_layout);
    QWidget *search_result=new QWidget;
    search_result->setStyleSheet("background-color: rgba(255,255,255,0);");
    search_result->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    QScrollArea *search_scroll=new QScrollArea;
    search_scroll->setStyleSheet("background-color: rgba(255,255,255,0);");
    search_scroll->setWidget(search_result);
    search_scroll->setWidgetResizable(true);
    QWidget *searchpage2=new QWidget;
    QVBoxLayout *page2layout=new QVBoxLayout;
    searchpage2->setLayout(page2layout);

    QStackedWidget* search_pages=new QStackedWidget;
    search_pages->addWidget(recommend_page);
    search_pages->addWidget(searchpage2);
    search_pages->setCurrentIndex(0);

    QPushButton *back=new QPushButton("返回");
    back->setFixedWidth(60);
    connect(back,&QPushButton::clicked,[=](){
        search_pages->setCurrentIndex(0);
    });

    page2layout->addWidget(search_scroll,13);
    page2layout->addWidget(back, 1, Qt::AlignLeft);

    //搜索
    connect(search_confirm,&QPushButton::clicked,[=](){
        //搜索算法 TODO
        QString keyword = search_column->text().trimmed();

        QJsonObject fieldQueries;
        if (!keyword.isEmpty()) {
            fieldQueries["title"] = keyword;
            fieldQueries["author"] = keyword;
            fieldQueries["description"] = keyword;
        }
        if (!selectedTags.isEmpty()) {
            QString tags = "";
            for (const QString& tag : selectedTags) {
                tags = tags + " " + tag;
            }
            fieldQueries["tags"] = tags;
        }

        QUrl url("http://localhost:8080/api/books/advanced-search");
        QJsonDocument doc(fieldQueries);
        QByteArray data = doc.toJson();

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *reply = networkManager->post(request, data);

        connect(reply, &QNetworkReply::finished, [=](){
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QList<Book> books = parseSearchResults(response);

                // 去重
                QList<Book> deduplicatedBooks;
                QSet<QString> seenKeys; // 用于记录已存在的键

                for (const Book &book : books) {
                    QString key = book.isbn; // 自定义唯一键
                    if (!seenKeys.contains(key)) {
                        deduplicatedBooks.append(book);
                        seenKeys.insert(key);
                    }
                }

                displaySearchResults(deduplicatedBooks, search_result);

            } else {
                qWarning() << "Search failed:" << reply->errorString();
            }
            reply->deleteLater();
        });

        search_pages->setCurrentIndex(1);
    });

    QVBoxLayout *layout3=new QVBoxLayout;
    layout3->setContentsMargins(30,35,30,35);
    layout3->addLayout(search_layout,1);
    layout3->addWidget(search_pages,14);

    pg->setLayout(layout3);
}



void MainWindow::saveAnnouncementsToFile(const QList<Announcement> &announcements) {
    QJsonArray array;
    for(const Announcement &ann : announcements) {
        array.append(ann.toJson());
    }

    QFile file("announcements.json");

    // 如果文件不存在会自动创建
    if(file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(array);
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "无法打开文件进行写入:" << file.errorString();
    }
}

QList<Announcement> MainWindow::readAllAnnouncements() {
    QList<Announcement> announcements;

    QFile file("announcements.json");

    // 如果文件不存在，返回空列表
    if(!file.exists()) {
        return announcements;
    }

    if(file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

        // 检查是否是有效JSON数组
        if(doc.isArray()) {
            QJsonArray array = doc.array();

            for(const QJsonValue &value : array) {
                if(value.isObject()) {
                    announcements.append(Announcement::fromJson(value.toObject()));
                }
            }
        }

        file.close();
    } else {
        qDebug() << "无法打开文件进行读取:" << file.errorString();
    }

    return announcements;
}

QWidget* MainWindow::createAnnouncementWidget(const Announcement& announcement) {
    QWidget* widget = new QWidget();
    widget->setStyleSheet("background-color: white; border-radius: 10px; padding: 15px; margin-bottom: 10px;");

    QVBoxLayout* layout = new QVBoxLayout(widget);

    // 用户信息行
    QHBoxLayout* userLayout = new QHBoxLayout();

    QLabel* userLabel = new QLabel(announcement.user());
    userLabel->setStyleSheet("font-weight: bold; color: #333;");

    QLabel* timeLabel = new QLabel(announcement.time().toString("yyyy-MM-dd hh:mm"));
    timeLabel->setStyleSheet("color: #999; font-size: 12px;");

    userLayout->addWidget(userLabel);
    userLayout->addStretch();
    userLayout->addWidget(timeLabel);

    // 公告内容
    QLabel* contentLabel = new QLabel(announcement.content());
    contentLabel->setWordWrap(true);
    contentLabel->setStyleSheet("color: #555; margin-top: 8px;");

    layout->addLayout(userLayout);
    layout->addWidget(contentLabel);

    return widget;
}

void MainWindow::refreshAnnouncements(QVBoxLayout* scrollLayout) {
    // 清除现有公告（保留其他可能的控件）
    QLayoutItem* item;
    while ((item = scrollLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // 读取所有公告
    QList<Announcement> announcements = readAllAnnouncements();

    // 添加公告到布局
    for (const Announcement& ann : announcements) {
        QWidget* announcementWidget = createAnnouncementWidget(ann);
        scrollLayout->addWidget(announcementWidget);
    }

    // 添加伸缩项使内容顶部对齐
    scrollLayout->addStretch();
}

void MainWindow::setpage4(QWidget* pg){
    QLabel *community_title=new QLabel("未名湖捞书社区");
    community_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    community_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );
    ResizeButton *add_announcement=new ResizeButton("发布新公告");

    QHBoxLayout *head_layout=new QHBoxLayout;
    head_layout->setContentsMargins(0,0,200,0);
    head_layout->setSpacing(30);
    head_layout->addWidget(community_title);
    head_layout->addWidget(add_announcement);

    // 滚动区域
    QVBoxLayout* scroll_layout = new QVBoxLayout;
    QWidget* scroll_page = new QWidget;
    scroll_page->setStyleSheet("background-color: rgba(255,255,255,0);");
    scroll_page->setLayout(scroll_layout);
    QScrollArea* scroll = new QScrollArea;
    scroll->setStyleSheet("background-color: rgba(255,255,255,0);");
    scroll->setWidget(scroll_page);
    scroll->setWidgetResizable(true);

    refreshAnnouncements(scroll_layout);

    connect(add_announcement,&QPushButton::clicked,[=](){
        if (currentUser == nullptr) QMessageBox::critical(this, "错误", "未登陆");
        else {
            AnnouncementDialog *new_announcement=new AnnouncementDialog(this);
            new_announcement->show();
            if(new_announcement->exec()== QDialog::Accepted){
                QString str = QString::fromStdString(currentUser->username);
                QString content = new_announcement->content();
                Announcement *ann = new Announcement(str, content);

                QList<Announcement> allAnnouncements = readAllAnnouncements();

                allAnnouncements.prepend(*ann);
                saveAnnouncementsToFile(allAnnouncements);
                QMessageBox::information(this, "成功", "公告已保存");
            }
            refreshAnnouncements(scroll_layout);
        }
    });

    QVBoxLayout *layout4=new QVBoxLayout;
    layout4->setContentsMargins(30,35,30,35);
    layout4->addLayout(head_layout,1);
    layout4->addStretch();
    layout4->addWidget(scroll,5);

    pg->setLayout(layout4);
}


void MainWindow::refreshOwnerbook(QVBoxLayout* scrollLayout){
    // 清除现有公告（保留其他可能的控件）
    QLayoutItem* item;
    while ((item = scrollLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    if(!bs.books.isEmpty()){
        for(auto book:bs.books){
            if((book!=nullptr) && (currentUser!=nullptr)){
                if((book->sellerId==QString::fromStdString(currentUser->username))&&!(book->isSold)){
                    QPushButton *bookbtn=new QPushButton(book->title);
                    bookbtn->setFixedHeight(50);
                    bookbtn->setStyleSheet(
                        "QPushButton{"
                        "background-color: rgba(73, 150, 255, 158);"
                        "color:white;"
                        "border:3px solid white;"
                        "border-radius:20px;"
                        " padding: 8px;"
                        "}"
                        "QPushButton:hover{"
                        "background-color: rgb(120, 170, 255);"
                        "}"
                    );
                    connect(bookbtn,&QPushButton::clicked,[=](){
                        MyBookDialog *display=new MyBookDialog(book,this);
                        display->show();
                        if(display->exec()==QDialog::Accepted){
                            bs.removeBook(book->isbn);
                            QMessageBox::information(this, "成功", "下架成功！");
                            updateUserInfo();
                            refreshOwnerbook(scrollLayout);
                        }
                        delete display;
                    });
                    scrollLayout->addWidget(bookbtn);
                }
            }
        }
    }
}

void MainWindow::setpage5(QWidget* pg){
    QLabel *sellbook_title=new QLabel("我的在售书目");
    sellbook_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sellbook_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );

    book_display=new QVBoxLayout;
    book_display->setSpacing(15);
    QWidget* display_page = new QWidget;
    display_page->setStyleSheet("background-color: rgba(255,255,255,0);");
    display_page->setLayout(book_display);
    display_page->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    QScrollArea *scroll_display=new QScrollArea;
    scroll_display->setStyleSheet("background-color: rgba(255,255,255,0);");
    scroll_display->setWidget(display_page);
    scroll_display->setWidgetResizable(true);

    refreshOwnerbook(book_display);

    ResizeButton *add_book=new ResizeButton("添加新书");
    //添加新书的后续操作 TODO
    connect(add_book,&QPushButton::clicked,[=](){
        FindbookDialog *addbook=new FindbookDialog(this);
        if (addbook->exec() == QDialog::Accepted) {
            if(!currentUser) QMessageBox::critical(this, "错误", "未登陆！");
            else{
                Book* book = new Book(addbook->getBook(QString::fromStdString(currentUser->username),bs.count()));
                if (bs.addBook(book)) {  // 假设addBook接受Book引用
                    if (bs.saveToFile()) {  // 保存到文件
                        QMessageBox::information(this, "成功", "上传成功！");
                        refreshOwnerbook(book_display);
                        updateUserInfo();
                    } else {
                        QMessageBox::critical(this, "错误", "保存文件失败！");
                    }
                } else {
                    QMessageBox::critical(this, "错误", "书籍已上传过！");
                    delete book;  // 添加失败时释放内存
                }
            }
        }
        addbook->deleteLater();  // 安全删除对话框
    });

    QHBoxLayout *headline=new QHBoxLayout;
    headline->addWidget(sellbook_title);
    headline->addStretch();
    headline->setSpacing(80);
    headline->addWidget(add_book);

    QVBoxLayout *layout5=new QVBoxLayout;
    layout5->setContentsMargins(30,35,30,35);
    layout5->addLayout(headline,1);
    layout5->addStretch();
    layout5->addWidget(scroll_display,5);

    pg->setLayout(layout5);
}



void MainWindow::openChatWindow(const QString &contact) {
    // 清除旧聊天界面
    QLayoutItem *child;
    while ((child = chatContainer->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // 创建新聊天界面
    QVBoxLayout *chatLayout = new QVBoxLayout(chatContainer);

    // 返回按钮
    QPushButton *backBtn = new QPushButton("← 返回联系人列表");
    backBtn->setStyleSheet("color: white; background: transparent; border: none;");
    connect(backBtn, &QPushButton::clicked, [this]() {
        chatContainer->setVisible(false);
    });
    chatLayout->addWidget(backBtn);

    // 聊天记录显示区域
    QTextBrowser *chatDisplay = new QTextBrowser;
    chatDisplay->setStyleSheet(
        "background: rgba(255,255,255,0.1);"
        "color: white;"
        "border: 1px solid #555;"
        "border-radius: 5px;"
        );

    // 加载聊天记录
    Chat chat(currentUser->username.c_str(), contact);
    for (const ChatMessage &msg : chat.messages) {
        QString msgHtml = QString("<div style='margin: 5px;'>"
                                  "<b style='color: %1'>%2 (%3):</b>"
                                  "<p>%4</p>"
                                  "</div>")
                              .arg(msg.sender == currentUser->username.c_str() ? "#88ff88" : "#ff8888")
                              .arg(msg.sender)
                              .arg(msg.timestamp.toString("hh:mm"))
                              .arg(msg.content.toHtmlEscaped());
        chatDisplay->append(msgHtml);
    }
    chatLayout->addWidget(chatDisplay);

    // 消息输入区域
    QHBoxLayout *inputLayout = new QHBoxLayout;
    QLineEdit *messageInput = new QLineEdit;
    messageInput->setStyleSheet(
        "background: rgba(255,255,255,0.2);"
        "color: white;"
        "border: 1px solid #555;"
        "border-radius: 5px;"
        "padding: 5px;"
        );

    QPushButton *sendBtn = new QPushButton("发送");
    sendBtn->setStyleSheet(
        "background: #4CAF50;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 5px 15px;"
        );

    inputLayout->addWidget(messageInput, 1);
    inputLayout->addWidget(sendBtn);
    chatLayout->addLayout(inputLayout);

    // 发送消息功能
    connect(sendBtn, &QPushButton::clicked, [this, contact, chatDisplay, messageInput]() {
        QString content = messageInput->text();
        if (!content.isEmpty()) {
            // 保存消息
            Chat chat(currentUser->username.c_str(), contact);
            chat.addMessage(currentUser->username.c_str(), content);

            // 更新显示
            QString msgHtml = QString("<div style='margin: 5px;'>"
                                      "<b style='color: #88ff88'>%1 (%2):</b>"
                                      "<p>%3</p>"
                                      "</div>")
                                  .arg(currentUser->username.c_str())
                                  .arg(QDateTime::currentDateTime().toString("hh:mm"))
                                  .arg(content.toHtmlEscaped());
            chatDisplay->append(msgHtml);

            messageInput->clear();
        }
    });

    // 显示聊天界面
    chatContainer->setVisible(true);
}


void MainWindow::setpage6(QWidget* pg){

    if (pg->layout()) {
        QLayoutItem *item;
        while ((item = pg->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete pg->layout();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(pg);

    if (currentUser == nullptr) {
        QLabel *loginHint = new QLabel("请先登录以查看联系人！");
        loginHint->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        loginHint->setStyleSheet("color: white; font-size: 30px;");
        mainLayout->addWidget(loginHint);
    }

    else {
        QLabel *title = new QLabel("我的联系人");
        title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        title->setStyleSheet(
            "color:white;"
            "font-size: 30px;"
            );
        mainLayout->addWidget(title);

        QWidget *contactsWidget = new QWidget;
        QVBoxLayout *contactsLayout = new QVBoxLayout(contactsWidget);

        QString username = QString::fromStdString(currentUser->username);
        QList<QString> contacts = Chat::getContacts(username);

        if (contacts.isEmpty()) {
            QLabel *hint = new QLabel("暂无联系人");
            hint->setStyleSheet("color: #aaa; font-size: 14px;");
            contactsLayout->addWidget(hint);
        }

        else {
            chatContainer = new QWidget(pg);
            chatContainer->setVisible(false);
            mainLayout->addWidget(chatContainer);

            for (const QString &contact : contacts) {
                QPushButton *contactBtn = new QPushButton(contact);
                contactBtn->setStyleSheet(
                    "QPushButton {"
                    "  background-color: rgba(255,255,255,0.1);"
                    "  color: white;"
                    "  border: 1px solid #555;"
                    "  padding: 10px;"
                    "  text-align: left;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: rgba(255,255,255,0.2);"
                    "}"
                    );

                connect(contactBtn, &QPushButton::clicked, [this, contact]() {
                    openChatWindow(contact);
                });

                contactsLayout->addWidget(contactBtn);
            }
            // 聊天容器初始化
            if (!chatContainer) {
                chatContainer = new QWidget(pg);
                chatContainer->setLayout(new QVBoxLayout());
                chatContainer->setVisible(false);
            }
            mainLayout->addWidget(chatContainer);

            pg->setLayout(mainLayout);

        }

        QScrollArea *contactsScroll = new QScrollArea;
        contactsScroll->setWidgetResizable(true);
        contactsScroll->setWidget(contactsWidget);
        contactsScroll->setStyleSheet("background: transparent; border: none;");

        mainLayout->addWidget(contactsScroll);
    }
}
