#-------------------------------------------------
#
# Project created by QtCreator 2012-01-14T20:53:56
#
#-------------------------------------------------

QT       += core gui xml

TARGET = spotwebfc
TEMPLATE = app

# ensure one "debug_and_release" in CONFIG, for clarity...
debug_and_release {
    CONFIG -= debug_and_release
    CONFIG += debug_and_release
}

# ensure one "debug" or "release" in CONFIG so they can be used as
# conditionals instead of writing "CONFIG(debug, debug|release)"...
CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug
}
CONFIG(release, debug|release) {
    CONFIG -= debug release
    CONFIG += release
}

release {
    OUTPUT_DIR = "release"
}

debug {
    OUTPUT_DIR = "debug"
}

win32 {
QMAKE_POST_LINK += xcopy /Y $${TARGET}.xml $${OUTPUT_DIR}
RC_FILE = spotwebfc.rc
}

SOURCES += main.cpp\
        mainwidget.cpp \
    aboutdialog.cpp \
    filtertreewidgetitem.cpp \
    newfilterdialog.cpp

HEADERS  += mainwidget.h \
    aboutdialog.h \
    filtertreewidgetitem.h \
    newfilterdialog.h

FORMS    += mainwidget.ui \
    aboutdialog.ui \
    newfilterdialog.ui

OTHER_FILES += \
    images/remove.png \
    images/filter.png \
    images/clear.png \
    images/add.png \
    images/spotwebfc.ico \
    spotwebfc.rc \
    images/filesaveas.png \
    images/filesave.png \
    images/fileload.png \
    spotwebfc.xml \
    images/about.png \
    images/picture.png \
    images/music.png \
    images/controller.png \
    images/application.png \
    images/film.png \
    images/eye.png \
    README

RESOURCES += \
    spotwebfc.qrc
