qt_internal_add_plugin(QPsdDescriptorLyidPlugin
    OUTPUT_NAME qlyid
    PLUGIN_TYPE psddescriptor
    SOURCES
        lyid.cpp
    LIBRARIES
        Qt::Core
        Qt::PsdCore
)
