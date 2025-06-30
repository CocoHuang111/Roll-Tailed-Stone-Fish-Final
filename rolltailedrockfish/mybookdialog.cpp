#include "mybookdialog.h"
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>

MyBookDialog::MyBookDialog(Book *book, QWidget *parent)  : QDialog(parent) ,book(book) {
    setWindowTitle("书本信息");
    display_title=new QLabel("书名："+book->title);
    display_author=new QLabel("作者："+book->author);
    display_publisher=new QLabel("出版商："+book->publisher);
    display_description=new QLabel("内容简介："+book->description);
    display_description->setWordWrap(true);
    display_price=new QLabel("售价："+QString::number(book->price));
    display_seller=new QLabel("卖家："+book->sellerId);
    removebook=new QPushButton("下架");
    exit=new QPushButton("取消");

    QVBoxLayout *message_layout=new QVBoxLayout;
    message_layout->addWidget(display_title);
    message_layout->addWidget(display_author);
    message_layout->addWidget(display_publisher);
    message_layout->addWidget(display_description);
    message_layout->addWidget(display_price);

    QHBoxLayout *lastline=new QHBoxLayout;
    lastline->addWidget(exit);
    lastline->addStretch();
    lastline->addWidget(removebook);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(message_layout);
    mainLayout->addStretch();
    mainLayout->addLayout(lastline);

    setLayout(mainLayout);
    setMaximumWidth(400);

    connect(removebook, &QPushButton::clicked, this, &QDialog::accept);
    connect(exit, &QPushButton::clicked, this, &QDialog::reject);
}
