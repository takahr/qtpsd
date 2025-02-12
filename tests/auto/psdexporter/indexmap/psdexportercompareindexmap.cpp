#include "psdexportercompareindexmap.h"

class PsdExporterCompareIndexMap::Private {
public:
    Private(PsdExporterCompareIndexMap *parent);
    ~Private();

    QHash<QString, QRect> convertToCompareRectMap(const QHash<const QPsdAbstractLayerItem *, QRect> &) const;
    QMultiMap<QString, QString> convertToCompareMergeMap(const QMultiMap<const QPsdAbstractLayerItem *, const QPsdAbstractLayerItem *> &) const;

    QHash<QString, QRect> convertToCompareRectMap(const QHash<const QPersistentModelIndex, QRect> &) const;
    QMultiMap<QString, QString> convertToCompareMergeMap(const QMultiMap<const QPersistentModelIndex, QPersistentModelIndex> &) const;

    static void findChildren(const QPsdAbstractLayerItem *item, QRect *rect);
    void generateRectMap(const QPsdAbstractLayerItem *item, const QPoint &topLeft) const;
    bool generateMergeData(const QPsdAbstractLayerItem *item) const;

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

QMultiMap<QString, QString> PsdExporterCompareIndexMap::Private::convertToCompareMergeMap(const QMultiMap<const QPersistentModelIndex, QPersistentModelIndex> &map) const
{
    QMultiMap<QString, QString> res;
    for (auto i = map.cbegin(); i != map.cend(); i++) {
        res.insert(QString::number(q->model()->layerId(i.key())), QString::number(q->model()->layerId(i.value())));
    }

    return res;
}

void PsdExporterCompareIndexMap::Private::findChildren(const QPsdAbstractLayerItem *item, QRect *rect)
{
    switch (item->type()) {
    case QPsdAbstractLayerItem::Folder: {
        const auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
        for (const auto *child : folder->children()) {
            findChildren(child, rect);
        }
        break; }
    default:
        *rect |= item->rect();
        break;
    }
}

void PsdExporterCompareIndexMap::Private::generateRectMap(const QPsdAbstractLayerItem *item, const QPoint &topLeft) const
{
    switch (item->type()) {
    case QPsdAbstractLayerItem::Folder: {
        const auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
        QRect contentRect;
        if (!item->parent()->parent()) {
            contentRect = item->parent()->rect();
            // if (item->parent() == tree) {
            //     contentRect = tree->rect();
        } else {
            for (const auto *child : folder->children()) {
                findChildren(child, &contentRect);
            }
        }
        q->rectMap.insert(item, contentRect.translated(-topLeft));
        for (const auto *child : folder->children()) {
            generateRectMap(child, contentRect.topLeft());
        }
        break; }
    default:
        q->rectMap.insert(item, item->rect().translated(-topLeft));
        break;
    }
}

bool PsdExporterCompareIndexMap::Private::generateMergeData(const QPsdAbstractLayerItem *item) const
{
    switch (item->type()) {
    case QPsdAbstractLayerItem::Folder: {
        const auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
        auto children = folder->children();
        std::reverse(children.begin(), children.end());
        for (const auto *child : children) {
            if (!generateMergeData(child))
                return false;
        }
        break; }
    default: {
        const auto hint = item->exportHint();
        if (hint.type != QPsdAbstractLayerItem::ExportHint::Merge)
            return true;
        const auto group = item->group();
        for (const auto *i : group) {
            if (i == item)
                continue;
            if (i->name() == hint.componentName) {
                q->mergeMap.insert(i, item);
            }
        }
        break; }
    }

    return true;
}

PsdExporterCompareIndexMap::PsdExporterCompareIndexMap(QObject *parent)
    : QPsdExporterPlugin{parent}, d(new Private(this))
{}

PsdExporterCompareIndexMap::~PsdExporterCompareIndexMap()
{}

bool PsdExporterCompareIndexMap::generateMaps(const PsdTreeItemModel *model) const
{
    setModel(model);

    rectMap.clear();
    mergeMap.clear();

    auto children = model->layerTree()->children();
    for (const auto *child : children) {
        d->generateRectMap(child, QPoint(0, 0));
        if (!d->generateMergeData(child))
            return false;
    }

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
