QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    edge.cpp \
    graphicsview.cpp \
    main.cpp \
    map.cpp \
    node.cpp \
    trpmainwindow.cpp

HEADERS += \
    edge.h \
    graphicsview.h \
    map.h \
    node.h \
    trpmainwindow.h

FORMS += \
    trpmainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

RC_ICONS ="icon.ico"
