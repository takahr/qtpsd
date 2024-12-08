# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationLfx2Plugin
    OUTPUT_NAME qlfx2
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        lfx2.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
