// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsdabstractplugin.h"

class QPsdAbstractPlugin::Private
{
public:
    QByteArray key;
};

QPsdAbstractPlugin::QPsdAbstractPlugin(QObject *parent)
    : QObject(parent)
    , d(new Private )
{}

QPsdAbstractPlugin::~QPsdAbstractPlugin() = default;

QByteArray QPsdAbstractPlugin::key() const
{
    return d->key;
}

void QPsdAbstractPlugin::setKey(const QByteArray &key)
{
    if (d->key == key) return;
    d->key = key;
    emit keyChanged(key);
}
