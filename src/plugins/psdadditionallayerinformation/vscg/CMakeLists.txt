# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationVscgPlugin
    OUTPUT_NAME qvscg
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        vscg.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
