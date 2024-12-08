# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdAdditionalLayerInformationV16DescriptorPlugin
    OUTPUT_NAME qv16descriptor
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        v16descriptor.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
