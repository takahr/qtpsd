// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtTest/QtTest>
#include <QtPsdCore/QPsdLayerRecord>
#include <QtPsdCore/QPsdLayerTreeItemModel>
#include <QtPsdCore/QPsdParser>

class tst_QPsdLayerTreeItemModel : public QObject
{
    Q_OBJECT
private slots:
    void parse();
};

void tst_QPsdLayerTreeItemModel::parse()
{
    QDir dir;
    dir.cd(QFINDTESTDATA("data/"_L1));

    QPsdParser parser;
    parser.load(dir.filePath("nested_layers.psd"));

    QPsdLayerTreeItemModel layerTree;
    layerTree.fromParser(parser);

    int rootRowCount = layerTree.rowCount(QModelIndex());
    QCOMPARE(rootRowCount, 1);

    QList<qint32> result;
    std::function<void(const QModelIndex &index)> traverseTreeView;
    traverseTreeView = [&](const QModelIndex &index) {
        result.append(layerTree.rowCount(index));

        if (index.isValid()) {
            const int layerId = layerTree.data(index, QPsdLayerTreeItemModel::Roles::LayerIdRole).value<int>();

            const auto *layerRecord = layerTree.data(index, QPsdLayerTreeItemModel::Roles::LayerRecordObjectRole).value<const QPsdLayerRecord *>();
            const auto additionalLayerInformation = layerRecord->additionalLayerInformation();

            // Layer ID
            const auto lyid = additionalLayerInformation.value("lyid").value<quint32>();

            QCOMPARE(layerId, lyid);
        }

        if (layerTree.hasChildren(index)) {
            for (int row = 0; row < layerTree.rowCount(index); row++) {
                traverseTreeView(layerTree.index(row, 0, index));
            }
        }
    };
    traverseTreeView(QModelIndex());

    QCOMPARE(result, QList<qint32>({ 1, 2, 3, 0, 2, 0, 0, 0, 3, 0, 2, 0, 0, 0 }));
}

QTEST_MAIN(tst_QPsdLayerTreeItemModel)
#include "tst_qpsdlayertreeitemmodel.moc"
