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

    bool exportTo(const PsdTreeItemModel *model, const QString &to, const QVariantMap &hint) const override;
};

bool QPsdExporterTmplPlugin::exportTo(const PsdTreeItemModel *model, const QString &to, const QVariantMap &hint) const
{
    std::function<void(const QModelIndex &, QDir *)> traverseTree = [&](const QModelIndex &index, QDir *directory) {
        bool isFolder = false;
        if (index.isValid()) {
            const auto *item = model->layerItem(index);
            switch (item->type()) {
            case QPsdAbstractLayerItem::Folder: {
                auto folder = dynamic_cast<const QPsdFolderLayerItem *>(item);
                directory->mkdir(folder->name());
                directory->cd(folder->name());
                isFolder = true;
                break; }
            case QPsdFolderLayerItem::Image:
                item->image().save(directory->filePath(item->name() + ".png"_L1), "PNG");
                break;
            default:
                break;
            }
        }
        
        for (int i = 0; i < model->rowCount(index); i++) {
            traverseTree(model->index(i, 0, index), directory);
        }

        if (isFolder) {
            directory->cdUp();
        }
    };

    QDir dir(to);
    traverseTree(QModelIndex{}, &dir);

    return true;
}

QT_END_NAMESPACE

#include "tmpl.moc"
