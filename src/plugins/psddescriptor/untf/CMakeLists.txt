# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdDescriptorUntFPlugin
    OUTPUT_NAME quntf
    PLUGIN_TYPE psddescriptor
    SOURCES
        untf.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
