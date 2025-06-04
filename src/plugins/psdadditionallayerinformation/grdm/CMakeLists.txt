# Copyright (C) 2025 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationGrdmPlugin
    OUTPUT_NAME qgrdm
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        grdm.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
