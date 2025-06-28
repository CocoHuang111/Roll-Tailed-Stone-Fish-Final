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
#include <QMessageBox>
#include "registerdialog.h"
#include "findbookdialog.h"
#include "Book.h"
#include "User.h"
#include "UserManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    ,background(":/image/background.jpg")
    ,networkManager(new QNetworkAccessManager(this))
    ,current_page(0)
{
    ui->setupUi(this);
    setAutoFillBackground(false);

    //左侧菜单栏
    QVBoxLayout *leftLayout=new QVBoxLayout;
    ResizeButton *btn1 = new ResizeButton("登陆/注册");
    ResizeButton *btn2 = new ResizeButton("我的主页");
    ResizeButton *btn3 = new ResizeButton("钓  书");
    ResizeButton *btn4 = new ResizeButton("捞  书");
    ResizeButton *btn5 = new ResizeButton("放  生");
    ResizeButton *btn6 = new ResizeButton("聊天墙");
    ResizeButton *btn7 = new ResizeButton("成就墙");
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
    QStackedWidget *MainArea=new QStackedWidget;
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
    //注册后的操作 TODO
    connect(startnew,&QPushButton::clicked,[=](){
        RegisterDialog *reg_diag = new RegisterDialog(this);
        reg_diag->show();
        bool flag = true;
        while(flag && reg_diag->exec() == QDialog::Accepted){
            if (qy.createUser(reg_diag->username, reg_diag->password, reg_diag->contact)) {
                flag = false;
                //ui->stackedWidget->setCurrentIndex(1);  // 跳转到登录页面
            } else {
                QMessageBox::critical(this, "错误", "用户名已存在！");
            }
        };
    });
    /*
    登录后的操作 TODO
    connect(startnew,&QPushButton::clicked,[=](){

    })*/

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

void MainWindow::setpage2(QWidget* pg){
    QLabel *title=new QLabel("个人信息");
    title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
    );
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLabel *username=new QLabel("用户名：");
    username->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );
    QLabel *contact=new QLabel("联系方式：");
    contact->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );
    QLabel *address=new QLabel("常用交易地址：");

    address->setStyleSheet(
        "background-color: rgba(120, 170, 255);"
        "color:white;"
        "border:3px solid white;"
        "border-radius:20px;"
        " padding: 8px;"
        "font-size: 20px;"
    );
    QLabel *book_on_sale=new QLabel("在售书籍：");
    book_on_sale->setStyleSheet(
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
    layout2->addWidget(username,1);
    //layout2->addStretch();
    layout2->addWidget(contact,1);
    //layout2->addStretch();
    layout2->addWidget(address,1);
    //layout2->addStretch();
    layout2->addWidget(book_on_sale,1);
    //layout2->addStretch();
    layout2->addWidget(record,1);
    //layout2->addStretch();
    layout2->addWidget(award,1);
    layout2->addStretch();

    pg->setLayout(layout2);
}

// TODO 返回isbn定位到书本身
QList<Book> MainWindow::parseSearchResults(const QByteArray &jsonData) {
    QList<Book> books;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isArray()) {
        for (const QJsonValue &val : doc.array()) {
            QJsonObject obj = val.toObject();
            Book book;
            book.title = obj["title"].toString();
            book.author = obj["author"].toString();
            book.price = obj["price"].toString().toDouble();
            // ...其他字段赋值
            books.append(book);
        }
    }
    return books;
}

// TODO 对display进行修改
void MainWindow::displaySearchResults(const QList<Book> &books, QWidget *container) {
    // 清空容器
    QLayout *layout = container->layout();
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
        QLabel *item = new QLabel(
            QString("<b>%1</b><br>作者: %2<br>价格: %3")
                .arg(book.title.toHtmlEscaped())
                .arg(book.author.toHtmlEscaped())
                .arg(book.price)
            );
        item->setStyleSheet("color: white; padding: 10px;");
        layout->addWidget(item);
    }
    container->setLayout(layout);
}

void MainWindow::setpage3(QWidget* pg){
    QLineEdit *search_column=new QLineEdit("搜索：");
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
    QLabel *guess_title=new QLabel("猜你喜欢：");
    guess_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    guess_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );
    //补充所有tag和猜你喜欢 TODO

    QVBoxLayout *recommend_layout=new QVBoxLayout;
    recommend_layout->addWidget(tag_title);
    recommend_layout->addWidget(guess_title);

    QWidget *recommend_page=new QWidget;
    recommend_page->setLayout(recommend_layout);
    QWidget *search_result=new QWidget;

    QStackedWidget* search_pages=new QStackedWidget;
    search_pages->addWidget(recommend_page);
    search_pages->addWidget(search_result);
    search_pages->setCurrentIndex(0);
    //搜索
    connect(search_confirm,&QPushButton::clicked,[=](){
        //搜索算法 TODO
        QString keyword = search_column->text().trimmed();

        // 构建请求URL
        QUrl url("http://localhost:8080/api/books/search");
        QUrlQuery query;
        query.addQueryItem("query", keyword);
        url.setQuery(query);

        // 发送GET请求
        QNetworkRequest request(url);
        QNetworkReply *reply = networkManager->get(request);

        connect(reply, &QNetworkReply::finished, [=](){
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QList<Book> books = parseSearchResults(response);
                displaySearchResults(books, search_result);
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

void MainWindow::setpage4(QWidget* pg){
    QLabel *community_title=new QLabel("未名湖捞书社区");
    community_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    community_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );

    QVBoxLayout *scroll_layout=new QVBoxLayout;
    //读入所有公告 TODO
    QWidget *scroll_page=new QWidget;
    scroll_page->setLayout(scroll_layout);
    QScrollArea *scroll=new QScrollArea;
    scroll->setWidget(scroll_page);
    scroll->setWidgetResizable(true);

    QVBoxLayout *layout4=new QVBoxLayout;
    layout4->setContentsMargins(30,35,30,35);
    layout4->addWidget(community_title);
    layout4->addStretch();
    layout4->addWidget(scroll);

    pg->setLayout(layout4);
}

void MainWindow::setpage5(QWidget* pg){
    QLabel *sellbook_title=new QLabel("我的在售书目");
    sellbook_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sellbook_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );

    QHBoxLayout *book_display=new QHBoxLayout;
    //读入在售书目的信息（图片等）TODO

    ResizeButton *add_book=new ResizeButton("添加新书");
    //添加新书的后续操作 TODO
    connect(add_book,&QPushButton::clicked,[=](){
        FindbookDialog addbook=new FindbookDialog(this);
        addbook.show();
    });

    QHBoxLayout *headline=new QHBoxLayout;
    headline->addWidget(sellbook_title);
    headline->addStretch();
    headline->setSpacing(80);
    headline->addWidget(add_book);

    QVBoxLayout *layout5=new QVBoxLayout;
    layout5->setContentsMargins(30,35,30,35);
    layout5->addLayout(headline);
    layout5->addStretch();
    layout5->addLayout(book_display);

    pg->setLayout(layout5);
}

void MainWindow::setpage6(QWidget* pg){
    QLabel *user_title=new QLabel("我的联系人");
    user_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    user_title->setStyleSheet(
        "color:white;"
        "font-size: 30px;"
        );
    QVBoxLayout *user_layout=new QVBoxLayout;
    user_layout->addWidget(user_title);
    //读入联系人信息，跳转聊天界面 TODO
    QScrollArea *scroll_user=new QScrollArea;
    scroll_user->setLayout(user_layout);

    QVBoxLayout *chat_layout=new QVBoxLayout;
    //聊天界面，读取聊天记录
    QScrollArea *scroll_chat=new QScrollArea;
    scroll_chat->setLayout(chat_layout);

    QStackedWidget *chat_pages=new QStackedWidget;
    chat_pages->addWidget(scroll_user);
    chat_pages->addWidget(scroll_chat);
    chat_pages->setCurrentIndex(0);

    QVBoxLayout *layout6=new QVBoxLayout;
    layout6->addWidget(chat_pages);

    pg->setLayout(layout6);
}
