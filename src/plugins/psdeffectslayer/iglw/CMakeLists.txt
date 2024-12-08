# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdEffectsLayerIglwPlugin
    OUTPUT_NAME qiglw
    PLUGIN_TYPE psdeffectslayer
    SOURCES
        iglw.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
