qt_internal_add_plugin(QPsdAdditionalLayerInformationTmplPlugin
    OUTPUT_NAME qtmpl
    PLUGIN_TYPE psdadditionallayerinformation
    SOURCES
        tmpl.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
