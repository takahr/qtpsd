// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpsddescriptor.h"
#include "qpsddescriptorplugin.h"

#include <QtCore/QBuffer>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQPsdDescriptor, "qt.psdcore.descriptor")

class QPsdDescriptor::Private : public QSharedData
{
public:
    QString name;
    QByteArray classID;
    QHash<QByteArray, QVariant> data;
    void parse(QIODevice *source, quint32 *length);
};

void QPsdDescriptor::Private::parse(QIODevice *source, quint32 *length)
{
    // Descriptor structure
    // https://www.adobe.com/devnet-apps/photoshop/fileformatashtml/#50577411_21585

    // Unicode string: name from classID
    name = readString(source, length);

    // classID: 4 bytes (length), followed either by string or (if length is zero) 4-byte classID
    auto size = readS32(source, length);
    classID = readByteArray(source, size == 0 ? 4 : size, length);

    auto count = readS32(source, length);

    qCDebug(lcQPsdDescriptor) << name << classID << count;
    while (count-- > 0) {
        qCDebug(lcQPsdDescriptor) << count;
        auto size = readS32(source, length);
        QByteArray key = readByteArray(source, size == 0 ? 4 : size, length);
        auto osType = readByteArray(source, 4, length);
        // load plugin for osType
        auto plugin = QPsdDescriptorPlugin::plugin(osType);
        if (plugin) {
            auto value = plugin->parse(source, length);
            data.insert(key, value);
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

QPsdDescriptor::QPsdDescriptor()
    : QPsdSection()
    , d(new Private)
{}

QPsdDescriptor::QPsdDescriptor(const QByteArray &data, int version)
    : QPsdDescriptor()
{
    QByteArray dataCopy = data;
    QBuffer buffer(&dataCopy);
    buffer.open(QIODevice::ReadOnly);
    quint32 length = data.size();
    if (version >= 0) {
        // check first 16 bytes for version
        quint32 v = readU32(&buffer, &length);
        if (v != static_cast<quint32>(version)) {
            qCWarning(lcQPsdDescriptor) << "Version mismatch: expected" << version << "but got" << v;
            return;
        }
    }
    d->parse(&buffer, &length);
}

QPsdDescriptor::QPsdDescriptor(QIODevice *source, quint32 *length)
    : QPsdDescriptor()
{
    d->parse(source, length);
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
