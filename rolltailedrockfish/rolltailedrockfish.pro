QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Announcement.cpp \
    announcementdialog.cpp \
    Book.cpp \
    BookShelf.cpp \
    Chat.cpp \
    displaydialog.cpp \
    findbookdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    User.cpp \
    UserManager.cpp \
    mybookdialog.cpp

HEADERS += \
    Announcement.h \
    announcementdialog.h \
    Book.h \
    BookShelf.h \
    Chat.h \
    displaydialog.h \
    findbookdialog.h \
    mainwindow.h \
    registerdialog.h \
    User.h \
    UserManager.h \
    mybookdialog.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
