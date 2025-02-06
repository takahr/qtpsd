#include "psdexportercompareindexmap.h"

class PsdExporterCompareIndexMap::Private {
public:
    Private(PsdExporterCompareIndexMap *parent);
    ~Private();

    QHash<QString, QRect> convertToCompareRectMap(const QHash<const QPsdAbstractLayerItem *, QRect> &) const;
    QMultiMap<QString, QString> convertToCompareMergeMap(const QMultiMap<const QPsdAbstractLayerItem *, const QPsdAbstractLayerItem *> &) const;

    QHash<QString, QRect> convertToCompareRectMap(const QHash<const QPersistentModelIndex, QRect> &) const;
    QMultiMap<QString, QString> convertToCompareMergeMap(const QMultiMap<const QPersistentModelIndex, const QPersistentModelIndex> &) const;

    PsdExporterCompareIndexMap *q;
};

PsdExporterCompareIndexMap::Private::Private(PsdExporterCompareIndexMap *parent) : q(parent)
{}

PsdExporterCompareIndexMap::Private::~Private()
{}

QHash<QString, QRect> PsdExporterCompareIndexMap::Private::convertToCompareRectMap(const QHash<const QPsdAbstractLayerItem *, QRect> &map) const
{
    QHash<QString, QRect> res;

    for (const auto &item: map.keys()) {
        res.insert(QString::number(item->id()), map.value(item));
    }

    return res;
}

QMultiMap<QString, QString> PsdExporterCompareIndexMap::Private::convertToCompareMergeMap(const QMultiMap<const QPsdAbstractLayerItem *, const QPsdAbstractLayerItem *> &map) const
{
    QMultiMap<QString, QString> res;
    for (auto i = map.cbegin(); i != map.cend(); i++) {
        res.insert(QString::number(i.key()->id()), QString::number(i.value()->id()));
    }

    return res;
}

QHash<QString, QRect> PsdExporterCompareIndexMap::Private::convertToCompareRectMap(const QHash<const QPersistentModelIndex, QRect> &map) const
{
    QHash<QString, QRect> res;

    for (auto i = map.constBegin(); i != map.constEnd(); i++) {
        res.insert(QString::number(q->model()->layerId(i.key())), i.value());
    }

    return res;
}

QMultiMap<QString, QString> PsdExporterCompareIndexMap::Private::convertToCompareMergeMap(const QMultiMap<const QPersistentModelIndex, const QPersistentModelIndex> &map) const
{
    QMultiMap<QString, QString> res;
    for (auto i = map.cbegin(); i != map.cend(); i++) {
        res.insert(QString::number(q->model()->layerId(i.key())), QString::number(q->model()->layerId(i.value())));
    }

    return res;
}

PsdExporterCompareIndexMap::PsdExporterCompareIndexMap(QObject *parent)
    : QPsdExporterPlugin{parent}, d(new Private(this))
{}

PsdExporterCompareIndexMap::~PsdExporterCompareIndexMap()
{}

bool PsdExporterCompareIndexMap::generateMaps(const PsdTreeItemModel *model) const
{
    setModel(model);
    return QPsdExporterPlugin::generateMaps();
}

bool PsdExporterCompareIndexMap::compareRectMap() const
{
    auto ca = d->convertToCompareRectMap(indexRectMap);
    auto cb = d->convertToCompareRectMap(rectMap);

    return ca == cb;
}

bool PsdExporterCompareIndexMap::compareMergeMap() const
{
    auto ca = d->convertToCompareMergeMap(indexMergeMap);
    auto cb = d->convertToCompareMergeMap(mergeMap);

    return ca == cb;
}
