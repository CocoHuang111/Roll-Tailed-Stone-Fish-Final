#include "findbookdialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>

FindbookDialog::FindbookDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("注册");

    // 创建控件
    QLabel *title = new QLabel("请填写书本信息！");
    booknameEdit = new QLineEdit; // 初始化成员变量
    QPushButton *loginBtn = new QPushButton("确认");

    // 连接槽函数
    connect(loginBtn, &QPushButton::clicked, this, &QDialog::accept);

    // 使用布局
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("书本名称：", booknameEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginBtn);

    setLayout(mainLayout);
    setFixedSize(300, 200);  // 可选：设置窗口大小
}
