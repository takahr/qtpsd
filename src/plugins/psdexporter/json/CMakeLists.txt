# Copyright (C) 2024 Signal Slot Inc.
# SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

qt_internal_add_plugin(QPsdExporterJsonPlugin
    OUTPUT_NAME qjson
    PLUGIN_TYPE psdexporter
    SOURCES
        json.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdExporter
)
