QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dms_adddocdialog.cpp \
    dms_cndtaddruledialog.cpp \
    dms_docbriefwidget.cpp \
    dms_helpaboutdialog.cpp \
    dms_horizontaldraglabel.cpp \
    dms_horizontaldragwidget.cpp \
    dms_managelabeldialog.cpp \
    dms_selectlabeldialog.cpp \
    dms_showdocinfodialog.cpp \
    dms_struct.cpp \
    main.cpp \
    mainwindow.cpp \
    virtualwinfuncs.cpp

HEADERS += \
    dms_adddocdialog.h \
    dms_cndtaddruledialog.h \
    dms_docbriefwidget.h \
    dms_helpaboutdialog.h \
    dms_horizontaldraglabel.h \
    dms_horizontaldragwidget.h \
    dms_managelabeldialog.h \
    dms_selectlabeldialog.h \
    dms_showdocinfodialog.h \
    dms_struct.h \
    mainwindow.h \
    virtualwinfuncs.h

TRANSLATIONS += \
    DMS_GUI_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lAdvAPI32
win32: LIBS += -lOle32
win32: LIBS += -lshell32
win32: LIBS += -lUser32
