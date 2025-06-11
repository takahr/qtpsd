# Copyright (C) 2025 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdDescriptorPthPlugin
    OUTPUT_NAME qpth
    PLUGIN_TYPE psddescriptor
    SOURCES
        pth.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
