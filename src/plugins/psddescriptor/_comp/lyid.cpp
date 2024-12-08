#include <QtPsdCore/qpsddescriptorplugin.h>
#include <QtPsdCore/qpsddescriptor.h>

QT_BEGIN_NAMESPACE

class QPsdDescriptorLyidPlugin : public QPsdDescriptorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdDescriptorFactoryInterface" FILE "lyid.json")
public:
    // Layer ID
    QVariant parse(QIODevice *source , quint32 length) const override {
        auto cleanup = qScopeGuard([&] {
            if (length == 2)
                skip(source, 2, &length); // a two byte null for the end of the string.
            Q_ASSERT(length == 0);
        });

        // ID.
        return readU32(source, &length);
    }
};

QT_END_NAMESPACE

#include "lyid.moc"
