#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

class RegisterDialog : public QDialog
{Q_OBJECT
public:
    RegisterDialog(QWidget *parent = nullptr);
    QString username;
    QString password;
    QString contact;
    QString address;
};

#endif // REGISTERDIALOG_H
