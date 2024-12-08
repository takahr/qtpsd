// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef PSDWIDGET_H
#define PSDWIDGET_H

#include <QtWidgets/QSplitter>
#include <QtPsdGui/QPsdFolderLayerItem>

class PsdWidget : public QSplitter
{
    Q_OBJECT
public:
    explicit PsdWidget(QWidget *parent = nullptr);
    ~PsdWidget() override;

    const QPsdFolderLayerItem *layerTree() const;

    QString fileName() const;

    QString errorMessage() const;
    QVariantMap exportHint(const QString& exporterKey) const;
    void updateExportHint(const QString &key, const QVariantMap &hint);

public slots:
    void load(const QString &fileName);
    void reload();
    void save();

private slots:
    void setErrorMessage(const QString &errorMessage);

signals:
    void errorOccurred(const QString &errorMessage);

private:
    class Private;
    QScopedPointer<Private> d;
};

#endif // PSDWIDGET_H
