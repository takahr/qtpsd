#ifndef PSDEXPORTERCOMPAREINDEXMAP_H
#define PSDEXPORTERCOMPAREINDEXMAP_H

#include <QtPsdExporter/qpsdexporterplugin.h>

class PsdExporterCompareIndexMap : public QPsdExporterPlugin
{
    Q_OBJECT
public:
    explicit PsdExporterCompareIndexMap(QObject *parent = nullptr);
    ~PsdExporterCompareIndexMap();

    virtual int priority() const override { return 0; }
    virtual QString name() const override { return u"test stub for CompareIndexMap"_s; }
    virtual ExportType exportType() const override { return ExportType::Directory; }
    virtual bool exportTo(const PsdTreeItemModel *model, const QString &to, const QVariantMap &hint) const override {
        return false;
    }

    bool generateMaps(const PsdTreeItemModel *model) const;

    bool compareRectMap() const;
    bool compareMergeMap() const;

private:
    class Private;
    QScopedPointer<Private> d;
};

#endif // PSDEXPORTERCOMPAREINDEXMAP_H
