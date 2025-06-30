#ifndef FINDBOOKDIALOG_H
#define FINDBOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "Book.h"

class FindbookDialog : public QDialog
{
public:
    FindbookDialog(QWidget *parent = nullptr);
    QStringList newtags;
    Book getBook(QString sellerid,int totalnum) ;
private:
    QLineEdit *titleEdit; // 需要将编辑框作为成员变量
    QLineEdit *authorEdit;
    QLineEdit *priceEdit;
    QLineEdit *publisherEdit;
    QLineEdit *descriptionEdit;

    QPushButton *add_tag1;
    QPushButton *add_tag2;
    QPushButton *add_tag3;
    QPushButton *add_tag4;
    QPushButton *add_tag5;
    QPushButton *add_tag6;
    QPushButton *add_tag7;
    QPushButton *add_tag8;
    QPushButton *add_tag9;

    QPushButton *add_tag2_1;
    QPushButton *add_tag2_2;
    QPushButton *add_tag2_3;
    QPushButton *add_tag2_4;
    QPushButton *add_tag2_5;

    QPushButton *loginBtn;
};

#endif // FINDBOOKDIALOG_H
