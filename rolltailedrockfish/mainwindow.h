#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonArray>
#include "Book.h"
#include "UserManager.h"
#include <QStackedWidget>
#include <QLabel>
#include "Announcement.h"
#include "BookShelf.h"
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ResizeButton : public QPushButton {
    Q_OBJECT
public:
    explicit ResizeButton(const QString& text, QWidget *parent = nullptr)
        : QPushButton(text, parent) {}

protected:
    void resizeEvent(QResizeEvent *event) override {
        QPushButton::resizeEvent(event);
        int newFontSize = event->size().height()/4.5 ;
        QFont f = font();
        f.setPointSize(newFontSize);
        setFont(f);
    }
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    UserManager qy;
    BookShelf bs;
    QStackedWidget *MainArea=new QStackedWidget;
    QVBoxLayout *book_display=nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPixmap background;
    QLabel* usernameLabel;
    QLabel* contactLabel;
    QLabel* onsalebookLabel;
    void updateUserInfo();
    QNetworkAccessManager *networkManager;
    int current_page;
    void setpage1(QWidget*);
    void setpage2(QWidget*);
    void setpage3(QWidget*);
    void setpage4(QWidget*);
    void setpage5(QWidget*);
    void setpage6(QWidget*);
    //void setpage7();

    ResizeButton *btn1;
    ResizeButton *btn2;
    ResizeButton *btn3;
    ResizeButton *btn4;
    ResizeButton *btn5;
    ResizeButton *btn6;
    ResizeButton *btn7;

    // page3 search相关
    QList<Book> parseSearchResults(const QByteArray &jsonData);
    void displaySearchResults(const QList<Book> &books, QWidget *container);
    //page4 announce相关
    void saveAnnouncementsToFile(const QList<Announcement> &ann);
    QList<Announcement> readAllAnnouncements();
    QWidget* createAnnouncementWidget(const Announcement& announcement);
    void refreshAnnouncements(QVBoxLayout* scrollLayout);
    //page5 在售书目
    void refreshOwnerbook(QVBoxLayout* scrollLayout);
    //page6 chat相关
    QWidget *chatContainer;
    void openChatWindow(const QString &contact);
    void updateContact();

};

#endif // MAINWINDOW_H
