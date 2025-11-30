cmake_minimum_required(VERSION 3.16)

project(spotwebfc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Gui Xml LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui Xml LinguistTools)

set(TS_FILES 
    spotwebfc_nl.ts
    spotwebfc_fr.ts
    spotwebfc_de.ts
    spotwebfc_xx.ts
)

set(PROJECT_SOURCES
    main.cpp
    mainwidget.cpp mainwidget.h mainwidget.ui
    aboutdialog.cpp aboutdialog.h aboutdialog.ui
    filtertreewidgetitem.cpp filtertreewidgetitem.h
    newfilterdialog.cpp newfilterdialog.h newfilterdialog.ui
    filterpropertiesdialog.cpp filterpropertiesdialog.h filterpropertiesdialog.ui
    settingsdialog.cpp settingsdialog.h settingsdialog.ui
    licensedialog.cpp licensedialog.h licensedialog.ui
    spotwebfc.rc
)

set(RESOURCE
    "spotwebfc.qrc"
)

qt5_add_resources(PROJECT_RESOURCES ${RESOURCE})

add_executable(spotwebfc
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(spotwebfc PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Xml
)

set_target_properties(spotwebfc PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS spotwebfc
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
