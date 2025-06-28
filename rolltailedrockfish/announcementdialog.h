#ifndef ANNOUNCEMENTDIALOG_H
#define ANNOUNCEMENTDIALOG_H

#include <QDialog>
#include <QLineEdit>

class AnnouncementDialog : public QDialog
{
public:
    AnnouncementDialog(QWidget *parent = nullptr);
    QString content() const;

private:
    QLineEdit *m_announcementEdit;
};

#endif // ANNOUNCEMENTDIALOG_H
