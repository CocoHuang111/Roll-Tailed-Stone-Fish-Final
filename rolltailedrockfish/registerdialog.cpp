#include "registerdialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("注册");

    // 创建控件
    QLabel *title = new QLabel("欢迎注册翻尾石鱼！为了保证交易的顺利进行，请完成以下必要信息的填写！");
    QLineEdit *usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("请输入用户名");
    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    QLineEdit *ContactEdit = new QLineEdit;
    ContactEdit->setPlaceholderText("请输入联系方式");
    QLineEdit *addressEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("请输入常用交易地址");

    QPushButton *loginBtn = new QPushButton("确认");

    // 连接槽函数
    connect(loginBtn, &QPushButton::clicked, this, &QDialog::accept);

    // 使用布局
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("用户名：", usernameEdit);
    formLayout->addRow("密码：", passwordEdit);
    formLayout->addRow("联系方式：", ContactEdit);
    formLayout->addRow("常用交易地址：", addressEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginBtn);

    setLayout(mainLayout);
    setFixedSize(300, 200);  // 可选：设置窗口大小
}
