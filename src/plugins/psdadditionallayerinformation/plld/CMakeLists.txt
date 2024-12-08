# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationPlLdPlugin
    OUTPUT_NAME qplld
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        plld.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
