QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = ../build/bin
OBJECTS_DIR = ../build/obj
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
UI_DIR = ../build/ui

SOURCES += \
    main.cpp \
    widget.cpp \
    ../processmanagement.cpp \
    ../flow/flow.cpp \
    ../schedulers/scheduler.cpp \
    ../schedulers/schedulealgorithms.cpp \
    ../schedulers/process.cpp \
    ../CE_threads.cpp \
    ../config/config_loader.cpp

HEADERS += \
    widget.h \
    ../processmanagement.h \
    ../flow/flow.h \
    ../schedulers/scheduler.h \
    ../schedulers/schedulealgorithms.h \
    ../schedulers/process.h \
    ../CE_threads.h \
    ../config/config_loader.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
