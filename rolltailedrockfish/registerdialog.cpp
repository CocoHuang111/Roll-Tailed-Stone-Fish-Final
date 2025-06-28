#include "registerdialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("注册");
    // 初始化成员变量Add commentMore actions
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);

    contactEdit = new QLineEdit(this);
    contactEdit->setPlaceholderText("请输入联系方式");

    QPushButton *confirmBtn = new QPushButton("确认注册", this);

    // 布局
    QLabel *title = new QLabel("欢迎注册翻尾石鱼！为了保证交易的顺利进行，请完成以下必要信息的填写！");
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("用户名:", usernameEdit);
    formLayout->addRow("密码:", passwordEdit);
    formLayout->addRow("联系方式:", contactEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(confirmBtn);

    setLayout(mainLayout);

    connect(confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
}
