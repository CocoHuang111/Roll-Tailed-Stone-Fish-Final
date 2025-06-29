#ifndef FINDBOOKDIALOG_H
#define FINDBOOKDIALOG_H

#include <QDialog>

class FindbookDialog : public QDialog
{
public:
    FindbookDialog(QWidget *parent = nullptr);
    QString bookname;
};

#endif // FINDBOOKDIALOG_H
