QT       += core gui sql
RC_ICONS += icon/student.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addadmindialog.cpp \
    addstudialog.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    studentsql.cpp

HEADERS += \
    addadmindialog.h \
    addstudialog.h \
    logindialog.h \
    mainwindow.h \
    studentsql.h

FORMS += \
    addadmindialog.ui \
    addstudialog.ui \
    logindialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES +=
