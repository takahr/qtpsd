# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT DEFINED INSTALL_APPSDIR)
    set(INSTALL_APPSDIR "apps")
endif()

set(INSTALL_APPSDIR "${INSTALL_APPSDIR}/qtpsd/psdexporter")

set(PROJECT_VERSION_MAJOR QT6_VERSION_MAJOR)
set(PROJECT_VERSION_MINOR QT6_VERSION_MINOR)
set(PROJECT_VERSION_PATCH QT6_VERSION_PATCH)

qt_internal_add_executable(psdexporter
    SOURCES
        main.cpp
        mainwindow.h mainwindow.cpp mainwindow.ui
        psdwidget.h psdwidget.cpp psdwidget.ui
        psdview.h psdview.cpp
        psdabstractitem.h psdabstractitem.cpp
        psdtextitem.h psdtextitem.cpp
        psdshapeitem.h psdshapeitem.cpp
        psdimageitem.h psdimageitem.cpp
        psdfolderitem.h psdfolderitem.cpp
        itemexportsettingdialog.h itemexportsettingdialog.cpp itemexportsettingdialog.ui
        exportdialog.h exportdialog.cpp exportdialog.ui
        psdtreeitemmodel.h psdtreeitemmodel.cpp
    LIBRARIES
        Qt::Widgets
        Qt::PsdGui
)

set_target_properties(psdexporter PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(psdexporter PRIVATE
    Qt::Widgets
    Qt::PsdCore
    Qt::PsdGui
)

install(TARGETS psdexporter
    RUNTIME DESTINATION "${INSTALL_APPSDIR}"
    BUNDLE DESTINATION "${INSTALL_APPSDIR}"
    LIBRARY DESTINATION "${INSTALL_APPSDIR}"
)
