# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationU16Plugin
    OUTPUT_NAME qu16
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        u16.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
