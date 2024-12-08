// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDEFFECTSLAYERPLUGIN_H
#define QPSDEFFECTSLAYERPLUGIN_H

#include <QtPsdCore/qpsdcoreglobal.h>
#include <QtPsdCore/qpsdabstractplugin.h>

QT_BEGIN_NAMESPACE

#define QPsdEffectsLayerPluginFactoryInterface_iid "org.qt-project.Qt.QPsdEffectsLayerFactoryInterface"

class Q_PSDCORE_EXPORT QPsdEffectsLayerPlugin : public QPsdAbstractPlugin
{
    Q_OBJECT
public:
    explicit QPsdEffectsLayerPlugin(QObject *parent = nullptr);

    virtual QVariant parse(QByteArrayView key, QIODevice *source, quint32 *length) const = 0;

    static QByteArrayList keys() {
        return QPsdAbstractPlugin::keys<QPsdEffectsLayerPlugin>(QPsdEffectsLayerPluginFactoryInterface_iid, "psdeffectslayer");
    }
    static QPsdEffectsLayerPlugin *plugin(const QByteArray &key) {
        return QPsdAbstractPlugin::plugin<QPsdEffectsLayerPlugin>(QPsdEffectsLayerPluginFactoryInterface_iid, "psdeffectslayer", key);
    }
};

QT_END_NAMESPACE

#endif // QPSDEFFECTSLAYERPLUGIN_H
