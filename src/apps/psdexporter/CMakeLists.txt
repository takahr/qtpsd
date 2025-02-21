# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_app(psdexporter
    SOURCES
        main.cpp
        mainwindow.h mainwindow.cpp mainwindow.ui
        psdwidget.h psdwidget.cpp psdwidget.ui
        exportdialog.h exportdialog.cpp exportdialog.ui
    LIBRARIES
        Qt::Widgets
        Qt::PsdWidget
    ENABLE_AUTOGEN_TOOLS
        uic
)

target_link_libraries(psdexporter PRIVATE
    Qt::Widgets
    Qt::PsdCore
    Qt::PsdGui
)
