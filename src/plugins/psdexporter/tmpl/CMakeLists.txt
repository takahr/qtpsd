qt_internal_add_plugin(QPsdExporterTmplPlugin
    OUTPUT_NAME qtmpl
    PLUGIN_TYPE psdexporter
    SOURCES
        tmpl.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdExporter
)
