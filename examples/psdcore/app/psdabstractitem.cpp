// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#include "psdabstractitem.h"
#include "psdfolderitem.h"

#include <QtGui/QPainter>

class PsdAbstractItem::Private
{
public:
    Private(const QPsdAbstractLayerItem *layer, PsdAbstractItem *parent);
    Private(const QPsdLayerRecord &record, PsdAbstractItem *parent);

private:
    PsdAbstractItem *q;
public:
    const QPsdAbstractLayerItem *layer = nullptr;
};

PsdAbstractItem::Private::Private(const QPsdAbstractLayerItem *layer, PsdAbstractItem *parent)
    : q(parent)
    , layer(layer)
{
    q->setVisible(layer->isVisible());
    q->setGeometry(layer->rect());
}

PsdAbstractItem::PsdAbstractItem(const QPsdAbstractLayerItem *layer, QWidget *parent)
    : QWidget(parent)
    , d(new Private(layer, this))
{}

PsdAbstractItem::~PsdAbstractItem() = default;

quint32 PsdAbstractItem::id() const
{
    return d->layer->id();
}

QString PsdAbstractItem::name() const
{
    return d->layer->name();
}

void PsdAbstractItem::setMask(QPainter *painter) const
{
    const QPsdAbstractLayerItem *layer = d->layer;
    while (layer) {
        // qDebug() << layer->name() << layer->vectorMask().elementCount();
        if (layer->vectorMask().type != QPsdAbstractLayerItem::PathInfo::None) {
            painter->setClipPath(layer->vectorMask().path, Qt::IntersectClip);
        }
        layer = layer->parent();
    }
    if (d->layer && d->layer->maskItem()) {
        QPixmap pixmap(size());
        pixmap.fill(Qt::transparent);
        const auto maskItem = d->layer->maskItem();
        const QImage maskImage = maskItem->transparencyMask();
        const auto intersected = maskItem->rect().intersected(geometry());
        QPainter p(&pixmap);
        p.drawImage(intersected.translated(-x(), -y()), maskImage, intersected.translated(-maskItem->rect().x(), -maskItem->rect().y()));
        p.end();

        painter->setClipRegion(QRegion(pixmap.createHeuristicMask()), Qt::IntersectClip);
    }
}

const QPsdAbstractLayerItem *PsdAbstractItem::abstractLayer() const
{
    return d->layer;
}
