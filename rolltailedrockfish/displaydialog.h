#ifndef DISPLAYDIALOG_H
#define DISPLAYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "Book.h"

class DisplayDialog : public QDialog
{
public:
    DisplayDialog(Book *book=nullptr, QWidget *parent = nullptr);
    QPushButton *contact_seller;
    Book *book;

private:
    QLabel *display_title;
    QLabel *display_author;
    QLabel *display_publisher;
    QLabel *display_description;
    QLabel *display_price;
    QPushButton *exit;
};

#endif // DISPLAYDIALOG_H
