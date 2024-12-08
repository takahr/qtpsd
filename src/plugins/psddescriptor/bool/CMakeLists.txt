# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdDescriptorBoolPlugin
    OUTPUT_NAME qbool
    PLUGIN_TYPE psddescriptor
    SOURCES
        bool.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
