# Copyright (C) 2025 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationPhflPlugin
    OUTPUT_NAME qphfl
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        phfl.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
