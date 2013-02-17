#
# Spotweb Filter Creator
# Copyleft 2012
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#-------------------------------------------------
#
# Project created by QtCreator 2012-01-14T20:53:56
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
QMAKE_POST_LINK += xcopy /Y $${TARGET}.cxml $${OUTPUT_DIR}
RC_FILE = spotwebfc.rc
}

unix {
QMAKE_LFLAGS += -Wl,-rpath,\\$\$ORIGIN/lib/:\\$\$ORIGIN/../lib/
}

SOURCES += main.cpp\
        mainwidget.cpp \
    aboutdialog.cpp \
    filtertreewidgetitem.cpp \
    newfilterdialog.cpp \
    filterpropertiesdialog.cpp \
    settingsdialog.cpp \
    licensedialog.cpp

HEADERS  += mainwidget.h \
    aboutdialog.h \
    filtertreewidgetitem.h \
    newfilterdialog.h \
    filterpropertiesdialog.h \
    settingsdialog.h \
    licensedialog.h

FORMS    += mainwidget.ui \
    aboutdialog.ui \
    newfilterdialog.ui \
    filterpropertiesdialog.ui \
    settingsdialog.ui \
    licensedialog.ui

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
    spotwebfc.cxml \
    images/about.png \
    images/picture.png \
    images/music.png \
    images/controller.png \
    images/application.png \
    images/film.png \
    images/eye.png \
    README \
    images/xbox.png \
    images/wmv.png \
    images/windows.png \
    images/vista.png \
    images/tv.png \
    images/tag.png \
    images/refresh.png \
    images/playstation.png \
    images/phone.png \
    images/nintendo_wii.png \
    images/nintendo_ds.png \
    images/mpg.png \
    images/mac.png \
    images/linux.png \
    images/ipod.png \
    images/home.png \
    images/hd.png \
    images/female.png \
    images/divx.png \
    images/custom.png \
    images/book.png \
    images/bluray.png \
    images/pda.png \
    images/dvd.png \
    images/today.png \
    images/fav.png \
    images/download.png \
    images/help.png \
    images/settings.png \
    spotwebfc_nl.ts \
    spotwebfc_de.ts \
    gpl-3.0.txt

RESOURCES += \
    spotwebfc.qrc

TRANSLATIONS += \
    spotwebfc_nl.ts \
    spotwebfc_de.ts \
    spotwebfc_xx.ts
