#include "findbookdialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>

FindbookDialog::FindbookDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("添加新书");

    // 创建控件
    QLabel *title = new QLabel("请填写书本信息！");
    QLabel *tagtitle=new QLabel("请选择tag：");
    titleEdit = new QLineEdit; // 初始化成员变量
    authorEdit= new QLineEdit;
    priceEdit= new QLineEdit;
    publisherEdit= new QLineEdit;
    descriptionEdit= new QLineEdit;

    QStringList add_tag1_content={"数学","计算机","中文","物理","化学","生物","政治","英语","经管"};
    QHBoxLayout *add_tagsline1=new QHBoxLayout;
    QPushButton *add_tag1=new QPushButton("数学");
    QPushButton *add_tag2=new QPushButton("计算机");
    QPushButton *add_tag3=new QPushButton("中文");
    QPushButton *add_tag4=new QPushButton("物理");
    QPushButton *add_tag5=new QPushButton("化学");
    QPushButton *add_tag6=new QPushButton("生物");
    QPushButton *add_tag7=new QPushButton("政治");
    QPushButton *add_tag8=new QPushButton("英语");
    QPushButton *add_tag9=new QPushButton("经管");
    QList<QPushButton*> add_alltag1={add_tag1,add_tag2,add_tag3,add_tag4,add_tag5,add_tag6,add_tag7,add_tag8,add_tag9};
    for(auto tag :add_alltag1){
        tag->setStyleSheet(
            "background-color:white;"
            );
    }
    for(int k=0;k<9;k++){
        add_tagsline1->addWidget(add_alltag1[k]);
        connect(add_alltag1[k],&QPushButton::clicked,[=](){
            newtags<<add_tag1_content[k];
            add_alltag1[k]->setStyleSheet(
                "background-color: gray;"
                );
        });
    }

    QStringList add_tag2_content={"教材","习题","研究","传记","文学"};
    QHBoxLayout *add_tagsline2=new QHBoxLayout;
    QPushButton *add_tag2_1=new QPushButton("教材");
    QPushButton *add_tag2_2=new QPushButton("习题");
    QPushButton *add_tag2_3=new QPushButton("研究");
    QPushButton *add_tag2_4=new QPushButton("传记");
    QPushButton *add_tag2_5=new QPushButton("文学");
    QList<QPushButton*> add_alltag2={add_tag2_1,add_tag2_2,add_tag2_3,add_tag2_4,add_tag2_5};
    for(auto tag :add_alltag2){
        tag->setStyleSheet(
            "background-color:white;"
            );
    }
    for(int k=0;k<5;k++){
        add_tagsline2->addWidget(add_alltag2[k]);
        connect(add_alltag2[k],&QPushButton::clicked,[=](){
            newtags<<add_tag2_content[k];
            add_alltag2[k]->setStyleSheet(
                "background-color: gray;"
                );
        });
    }

    loginBtn = new QPushButton("确认");

    // 连接槽函数
    connect(loginBtn, &QPushButton::clicked, this, &QDialog::accept);

    // 使用布局
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("书本名称：", titleEdit);
    formLayout->addRow("作者：", authorEdit);
    formLayout->addRow("出版商：", publisherEdit);
    formLayout->addRow("内容简介：", descriptionEdit);
    formLayout->addRow("定价：", priceEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(tagtitle);
    mainLayout->addLayout(add_tagsline1);
    mainLayout->addLayout(add_tagsline2);
    mainLayout->addStretch();
    mainLayout->addWidget(loginBtn);

    setLayout(mainLayout);
    setFixedSize(600, 400);  // 可选：设置窗口大小
}

Book FindbookDialog::getBook(QString sellerid,int totalnum){
    QString newtitle=titleEdit->text();
    QString newauthor=authorEdit->text();
    QString newpublisher=publisherEdit->text();
    QString newdescription=descriptionEdit->text();
    double newprice=priceEdit->text().toDouble();
    return Book(newtitle,newauthor,newpublisher,0,QString::fromStdString(std::to_string(totalnum)),newtags,newprice,newdescription,"",QDateTime::currentDateTime(),sellerid);
}
