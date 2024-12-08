// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdCore/qpsddescriptorplugin.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorVlLsPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "vlls.json")
public:
    // List
    QVariant parse(QIODevice *source , quint32 *length) const override {
        // Number of items in the list
        auto count = readS32(source, length);
        QVariantList ret;
        for (int i = 0; i < count; ++i) {
            // OSType key for type to use. See See Descriptor structure for types.
            QByteArray osType = readByteArray(source, 4, length);
            auto plugin = QPsdDescriptorPlugin::plugin(osType);
            if (plugin) {
                auto value = plugin->parse(source, length);
                ret.append(value);
            } else {
                qWarning() << osType << "not supported";
                break;
            }
        }
        return ret;
    }
};

QT_END_NAMESPACE

#include "vlls.moc"
