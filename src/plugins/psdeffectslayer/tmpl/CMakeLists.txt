qt_internal_add_plugin(QPsdEffectsLayerTmplPlugin
    OUTPUT_NAME qtmpl
    PLUGIN_TYPE psdeffectslayer
    SOURCES
        tmpl.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
