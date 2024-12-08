# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdDescriptorEnumPlugin
    OUTPUT_NAME qenum
    PLUGIN_TYPE psddescriptor
    SOURCES
        enum.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
