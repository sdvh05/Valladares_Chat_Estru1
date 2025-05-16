QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Master.cpp \
    Usuario.cpp \
    amigosmanejo.cpp \
    chat.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    pantallainicio.cpp \
    recuperarpass.cpp \
    signin.cpp

HEADERS += \
    Master.h \
    Usuario.h \
    amigosmanejo.h \
    chat.h \
    login.h \
    mainwindow.h \
    pantallainicio.h \
    recuperarpass.h \
    signin.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
