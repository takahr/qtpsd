#include <QtPsdCore/qpsdeffectslayerplugin.h>

QT_BEGIN_NAMESPACE

class QPsdEffectsLayerTmplPlugin : public QPsdEffectsLayerPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdEffectsLayerFactoryInterface" FILE "tmpl.json")
public:
    QVariant parse(QIODevice *source , quint32 *length) const override {
        return QVariant();
    }
};

QT_END_NAMESPACE

#include "tmpl.moc"
