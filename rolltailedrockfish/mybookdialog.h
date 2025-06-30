#ifndef MYBOOKDIALOG_H
#define MYBOOKDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "Book.h"

class MyBookDialog : public QDialog
{
public:
    MyBookDialog(Book *book=nullptr, QWidget *parent = nullptr);
    //DisplayDialog(QWidget *parent = nullptr);
    QPushButton *removebook;
    Book *book;
    QLabel *display_title;
    QLabel *display_author;
    QLabel *display_publisher;
    QLabel *display_description;
    QLabel *display_price;
    QLabel *display_seller;
    QPushButton *exit;
};

#endif // MYBOOKDIALOG_H
