#ifndef FINDBOOKDIALOG_H
#define FINDBOOKDIALOG_H

#include <QDialog>

class FindbookDialog : public QDialog
{
public:
    FindbookDialog(QWidget *parent = nullptr);
    QString bookname;
    QString getBookname() const { return booknameEdit->text(); }
private:
    QLineEdit *booknameEdit; // 需要将编辑框作为成员变量
};

#endif // FINDBOOKDIALOG_H
