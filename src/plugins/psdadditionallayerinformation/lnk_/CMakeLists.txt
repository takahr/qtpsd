# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationLnk_Plugin
    OUTPUT_NAME qlnk_
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        lnk_.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
