#include <QtPsdGui/qpsdexporterplugin.h>
#include <QtCore/QDir>

QT_BEGIN_NAMESPACE

class QPsdExporterTmplPlugin : public QPsdExporterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPsdExporterFactoryInterface" FILE "tmpl.json")
public:
    int priority() const override { return 100; }
    QString name() const override {
        return tr("");
    }
    ExportType exportType() const override { return QPsdExporterPlugin::Directory; }

    bool exportTo(const QPsdFolderLayerItem *tree, const QString &to) const override;
};

bool QPsdExporterTmplPlugin::exportTo(const QPsdFolderLayerItem *tree, const QString &to) const
{
    std::function<void(const QPsdAbstractLayerItem *, QDir *)> traverseTree = [&](const QPsdAbstractLayerItem *item, QDir *directory) {
        switch (item->type()) {
        case QPsdAbstractLayerItem::Folder: {
            auto folder = reinterpret_cast<const QPsdFolderLayerItem *>(item);
            directory->mkdir(folder->name());
            directory->cd(folder->name());
            for (const auto *child : folder->children()) {
                traverseTree(child, directory);
            }
            directory->cdUp();
            break; }
        case QPsdFolderLayerItem::Image:
            item->image().save(directory->filePath(item->name() + ".png"_L1), "PNG");
            break;
        default:
            break;
        }
    };

    QDir dir(to);
    for (const auto *item : tree->children()) {
        traverseTree(item, &dir);
    }
    return true;
}

QT_END_NAMESPACE

#include "tmpl.moc"
