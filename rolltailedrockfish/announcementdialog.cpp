#include "announcementdialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>

AnnouncementDialog::AnnouncementDialog(QWidget *parent) : QDialog(parent)  {
    setWindowTitle("发布新公告");

    // 创建控件
    QLabel *title = new QLabel("请填写公告内容～");
    QLineEdit *AnnouncementEdit = new QLineEdit;
    announcement_content->AnnouncementEdit.text();
    QPushButton *loginBtn = new QPushButton("确认");

    // 连接槽函数
    connect(loginBtn, &QPushButton::clicked, this, &QDialog::accept);

    // 使用布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);
    mainLayout->addWidget(AnnouncementEdit);
    mainLayout->addWidget(loginBtn);

    setLayout(mainLayout);
    setFixedSize(400, 200);  // 可选：设置窗口大小
}
