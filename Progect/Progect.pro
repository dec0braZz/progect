QT += widgets sql
QT -= gui
QT += network
QMAKE_CXXFLAGS += -O2

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ColorPalette.cpp \
        chatwindow.cpp \
        main.cpp \
        okno.cpp \
        secondwindow.cpp \
        secondwindowregistr.cpp \
        setting.cpp \
        socket.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ColorPalette.h \
    chatwindow.h \
    okno.h \
    secondwindow.h \
    secondwindowregistr.h \
    setting.h \
    socket.h
