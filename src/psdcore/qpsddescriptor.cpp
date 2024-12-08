// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsddescriptor.h"
#include "qpsddescriptorplugin.h"

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQPsdDescriptor, "qt.psdcore.descriptor")

class QPsdDescriptor::Private : public QSharedData
{
public:
    QString name;
    QByteArray classID;
    QHash<QByteArray, QVariant> data;
};

QPsdDescriptor::QPsdDescriptor()
    : QPsdSection()
    , d(new Private)
{}

QPsdDescriptor::QPsdDescriptor(QIODevice *source, quint32 *length)
    : QPsdDescriptor()
{
    // Descriptor structure
    // https://www.adobe.com/devnet-apps/photoshop/fileformatashtml/#50577411_21585

    // Unicode string: name from classID
    d->name = readString(source, length);

    // classID: 4 bytes (length), followed either by string or (if length is zero) 4-byte classID
    auto size = readS32(source, length);
    Q_ASSERT(size < 20);
    d->classID = readByteArray(source, size == 0 ? 4 : size, length);

    auto count = readS32(source, length);

    qCDebug(lcQPsdDescriptor) << d->name << d->classID << count;
    while (count-- > 0) {
        qCDebug(lcQPsdDescriptor) << count;
        auto size = readS32(source, length);
        QByteArray key = readByteArray(source, size == 0 ? 4 : size, length);
        auto osType = readByteArray(source, 4, length);
        // load plugin for osType
        auto plugin = QPsdDescriptorPlugin::plugin(osType);
        if (plugin) {
            auto value = plugin->parse(source, length);
            d->data.insert(key, value);
            if (value.typeId() == QMetaType::QByteArray) {
                value = value.toByteArray().left(20);
            }
            qCDebug(lcQPsdDescriptor) << key << osType << value;
        } else {
            qCWarning(lcQPsdDescriptor) << osType << "not supported for" << key.left(4);
            break;
        }
    }
}

QPsdDescriptor::QPsdDescriptor(const QPsdDescriptor &other)
    : QPsdSection(other)
    , d(other.d)
{}

QPsdDescriptor &QPsdDescriptor::operator=(const QPsdDescriptor &other)
{
    if (this != &other) {
        QPsdSection::operator=(other);
        d.operator=(other.d);
    }
    return *this;
}

QPsdDescriptor::~QPsdDescriptor() = default;

QString QPsdDescriptor::name() const
{
    return d->name;
}

QByteArray QPsdDescriptor::classID() const
{
    return d->classID;
}

QHash<QByteArray, QVariant> QPsdDescriptor::data() const
{
    return d->data;
}

QDebug operator<<(QDebug s, const QPsdDescriptor &value)
{
    QDebugStateSaver saver(s);
    s.nospace() << "QPsdDescriptor(" << value.name() << ", " << value.classID() << ", " << value.data() << ")";
    return s;
}

QT_END_NAMESPACE
