# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationDataPlugin
    OUTPUT_NAME qdata
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        data.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
