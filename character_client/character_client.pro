QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_character_dialog.cpp \
    character_info_dialog.cpp \
    client_connection.cpp \
    main.cpp \
    main_window.cpp \
    protocol.cpp

HEADERS += \
    add_character_dialog.h \
    character_info_dialog.h \
    client_connection.h \
    main_window.h \
    protocol.h

FORMS += \
    add_character_dialog.ui \
    character_info_dialog.ui \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
