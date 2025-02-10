// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtPsdExporter/QPsdExporterPlugin>
#include "psdexportercompareindexmap.h"

#include <QtTest/QtTest>

class tst_QPsdExporter_IndexMap : public QObject {
    Q_OBJECT
private slots:
    void compareMap();
};

void tst_QPsdExporter_IndexMap::compareMap()
{
    QDir dir;
    dir.cd(QFINDTESTDATA("data/"_L1));

    PsdTreeItemModel model;
    model.load(dir.filePath("layer_link.psd"));

    PsdExporterCompareIndexMap comparePlugin;
    comparePlugin.generateMaps(&model);

    QVERIFY(comparePlugin.compareRectMap());
    QVERIFY(comparePlugin.compareMergeMap());
}

QTEST_MAIN(tst_QPsdExporter_IndexMap)
#include "tst_qpsdexporter_indexmap.moc"
