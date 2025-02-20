# Copyright (C) 2025 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#####################################################################
## PsdGui Module:
#####################################################################
qt_internal_add_module(PsdWidget
    SOURCES
        qpsdwidgetglobal.h
        psdview.h psdview.cpp
        psdabstractitem.h psdabstractitem.cpp
        psdtextitem.h psdtextitem.cpp
        psdshapeitem.h psdshapeitem.cpp
        psdimageitem.h psdimageitem.cpp
        psdfolderitem.h psdfolderitem.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_LIBRARIES
        Qt::Widgets
        Qt::PsdExporter
)

## Scopes:
#####################################################################
