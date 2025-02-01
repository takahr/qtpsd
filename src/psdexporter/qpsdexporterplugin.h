// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPSDEXPORTERPLUGIN_H
#define QPSDEXPORTERPLUGIN_H

#include <QtPsdExporter/qpsdexporterglobal.h>

#include "psdtreeitemmodel.h"

#include <QtPsdCore/qpsdabstractplugin.h>
#include <QtPsdGui/qpsdfolderlayeritem.h>
#include <QtPsdGui/qpsdtextlayeritem.h>
#include <QtPsdGui/qpsdshapelayeritem.h>
#include <QtPsdGui/qpsdimagelayeritem.h>

#include <QtCore/QMimeDatabase>
#include <QtGui/QIcon>

QT_BEGIN_NAMESPACE

#define QPsdExporterFactoryInterface_iid "org.qt-project.Qt.QPsdExporterFactoryInterface"

class Q_PSDEXPORTER_EXPORT QPsdExporterPlugin : public QPsdAbstractPlugin
{
    Q_OBJECT
public:
    enum ExportType {
        File,
        Directory,
    };
    explicit QPsdExporterPlugin(QObject *parent = nullptr);

    virtual int priority() const = 0;
    virtual QIcon icon() const { return QIcon(); }
    virtual QString name() const = 0;
    virtual ExportType exportType() const = 0;
    virtual QHash<QString, QString> filters() const { return {}; }

    virtual bool exportTo(const PsdTreeItemModel *model, const QString &to, const QVariantMap &hint) const = 0;

    static QByteArrayList keys() {
        return QPsdAbstractPlugin::keys<QPsdExporterPlugin>(QPsdExporterFactoryInterface_iid, "psdexporter");
    }
    static QPsdExporterPlugin *plugin(const QByteArray &key) {
        return QPsdAbstractPlugin::plugin<QPsdExporterPlugin>(QPsdExporterFactoryInterface_iid, "psdexporter", key);
    }

    static QString toUpperCamelCase(const QString &str, const QString &separator = QString());
protected:
    static QString toLowerCamelCase(const QString &str);
    static QString toSnakeCase(const QString &str);
    static QString toKebabCase(const QString &str);

    static QString imageFileName(const QString &name, const QString &format);
    bool generateMaps(const PsdTreeItemModel *model) const;

protected:
    static QMimeDatabase mimeDatabase;

    mutable QHash<const QPsdAbstractLayerItem *, QRect> rectMap;
    mutable QMultiMap<const QPsdAbstractLayerItem *, const QPsdAbstractLayerItem *> mergeMap;

private:
    static void findChildren(const QPsdAbstractLayerItem *item, QRect *rect);
    void generateRectMap(const QPsdAbstractLayerItem *item, const QPoint &topLeft) const;
    bool generateMergeData(const QPsdAbstractLayerItem *item) const;
};

QT_END_NAMESPACE

#endif // QPSDEXPORTERPLUGIN_H
