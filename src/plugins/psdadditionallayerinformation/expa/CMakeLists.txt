# Copyright (C) 2025 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationExpaPlugin
    OUTPUT_NAME qexpa
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        expa.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
