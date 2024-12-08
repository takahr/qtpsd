# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdDescriptorVlLsPlugin
    OUTPUT_NAME qvlls
    PLUGIN_TYPE psddescriptor
    SOURCES
        vlls.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
