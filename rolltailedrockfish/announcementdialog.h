#ifndef ANNOUNCEMENTDIALOG_H
#define ANNOUNCEMENTDIALOG_H

#include <QDialog>

class AnnouncementDialog : public QDialog
{
public:
    AnnouncementDialog(QWidget *parent = nullptr);
    QString announcement_content;
};

#endif // ANNOUNCEMENTDIALOG_H
