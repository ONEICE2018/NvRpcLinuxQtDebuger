QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT  +=  printsupport
CONFIG += c++11

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
    axistag.cpp \
    coordinatemap.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    widget_machine_status.cpp \
    widget_pid_curve.cpp

HEADERS += \
    axistag.h \
    coordinatemap.h \
    mainwindow.h \
    qcustomplot.h \
    widget_machine_status.h \
    widget_pid_curve.h

FORMS += \
    coordinatemap.ui \
    mainwindow.ui \
    widget_machine_status.ui \
    widget_pid_curve.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lrpclib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lrpclib
else:unix: LIBS += -L$$PWD/./ -lrpclib

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/librpclib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/librpclib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/rpclib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/rpclib.lib
else:unix: PRE_TARGETDEPS += $$PWD/./librpclib.a
