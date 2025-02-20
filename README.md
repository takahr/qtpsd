# PSD Parser Module for Qt 6

This module provides comprehensive functionality for parsing, displaying, and exporting Adobe Photoshop PSD files. It includes a core parsing library, GUI components, and a standalone exporter application.

## Features

### Core Features
- **Layer Parsing**: Extracts and organizes layers, including text, shapes, and image layers, in a tree structure
- **Layer Metadata**: Access additional layer information, such as blending modes, opacity, and linking relationships
- **Effects Support**: Handle layer effects including shadows, bevels, glows, and more
- **Plugin Architecture**: Extensible system for handling additional layer information and effects
- **Cross-Platform**: Fully compatible with any Qt 6-supported platform

### Components
- **PsdCore**: Core library for parsing and handling PSD files
- **PsdGui**: Qt widgets for displaying PSD layer trees and content
- **PsdExporter**: Standalone application and library for exporting PSD files
- **Plugins**: Various plugins for handling additional layer information, effects, and descriptors

## Requirements

- Qt 6.8 or later

## Installation

### Clone the repository:

```console
$ git clone https://github.com/signal-slot/qtpsd.git
$ cd qtpsd
```

### Create a build directory and configure the project:

```console
$ mkdir build && cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/your/qt/project
```

### Build and install the module:

```console
$ cmake --build .
$ cmake --install .
```

## Applications

### PSD Exporter
A GUI application for viewing and exporting PSD files is included. Build with:

```console
$ cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/your/qt/project -DQT_BUILD_EXAMPLES=ON
$ cmake --build .
$ ./src/apps/psdexporter/psdexporter
```

### Demo Application
A simple demo application showing core functionality:

```console
$ cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/your/qt/project -DQT_BUILD_EXAMPLES=ON
$ cmake --build .
$ ./examples/psdcore/psdinfo/psdinfo
```

## Using QtPsd in Your Project

### Link the modules to your Qt project:

```cmake
find_package(Qt6 COMPONENTS PsdCore PsdGui PsdExporter REQUIRED)
target_link_libraries(your_app PRIVATE
    Qt::PsdCore
    Qt::PsdGui
    Qt::PsdExporter
)
```

### Basic Usage Example:

```cpp
#include <QtPsdCore>
#include <QtPsdGui>

// Parse a PSD file
QPsdParser parser;
if (parser.parse("input.psd")) {
    // Access the layer tree
    auto model = parser.layerTreeModel();
    
    // Display in a Qt widget
    QPsdLayerTree view;
    view.setModel(model);
    view.show();
}
```

## License

This module is licensed under the LGPLv3, GPLv2 or GPLv3 License.
