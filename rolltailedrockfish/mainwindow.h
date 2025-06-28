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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPixmap background;
    QNetworkAccessManager *networkManager;
    int current_page;
    void setpage1(QWidget*);
    void setpage2(QWidget*);
    void setpage3(QWidget*);
    void setpage4(QWidget*);
    void setpage5(QWidget*);
    void setpage6(QWidget*);
    /*
    void setpage6();
    void setpage7();
*/
    // page3 search相关
    QList<Book> parseSearchResults(const QByteArray &jsonData);
    void displaySearchResults(const QList<Book> &books, QWidget *container);
};

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

#endif // MAINWINDOW_H
