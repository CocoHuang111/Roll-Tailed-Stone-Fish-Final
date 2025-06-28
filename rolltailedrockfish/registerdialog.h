#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

class RegisterDialog : public QDialog
{Q_OBJECT
public:
    RegisterDialog(QWidget *parent = nullptr);
    QString getUsername() const { return usernameEdit ? usernameEdit->text() : QString(); }
    QString getPassword() const { return passwordEdit ? passwordEdit->text() : QString(); }
    QString getContact() const { return contactEdit ? contactEdit->text() : QString(); }

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *contactEdit;
};

#endif // REGISTERDIALOG_H
