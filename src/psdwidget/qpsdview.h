// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef PSDVIEW_H
#define PSDVIEW_H

#include <qpsdwidgetglobal.h>
#include <QtPsdExporter/psdtreeitemmodel.h>

#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Q_PSDWIDGET_EXPORT QPsdView : public QWidget
{
    Q_OBJECT
public:
    QPsdView(QWidget *parent = nullptr);
    ~QPsdView() override;

    QPsdTreeItemModel *model() const;

public slots:
    void setModel(QPsdTreeItemModel *model);
    void setItemVisible(quint32 id, bool visible);
    void reset();
    void clearSelection();

signals:
    void itemSelected(const QModelIndex &index);
    void modelChanged(QPsdTreeItemModel *model);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    class Private;
    QScopedPointer<Private> d;
};

QT_END_NAMESPACE

#endif // PSDVIEW_H
