// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef PSDVIEW_H
#define PSDVIEW_H

#include <QtPsdExporter/psdtreeitemmodel.h>

#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class PsdView : public QWidget
{
    Q_OBJECT
public:
    PsdView(QWidget *parent = nullptr);
    ~PsdView() override;

    PsdTreeItemModel *model() const;

public slots:
    void setModel(PsdTreeItemModel *model);
    void setItemVisible(quint32 id, bool visible);
    void reset();
    void clearSelection();

signals:
    void itemSelected(const QModelIndex &index);
    void modelChanged(PsdTreeItemModel *model);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    class Private;
    QScopedPointer<Private> d;
};

QT_END_NAMESPACE

#endif // PSDVIEW_H
