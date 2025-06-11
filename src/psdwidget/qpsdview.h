// Copyright (C) 2024 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QPSDVIEW_H
#define QPSDVIEW_H

#include <QtPsdWidget/qpsdwidgetglobal.h>
#include <QtPsdWidget/qpsdwidgettreeitemmodel.h>

#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Q_PSDWIDGET_EXPORT QPsdView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool showChecker READ showChecker WRITE setShowChecker NOTIFY showCheckerChanged)
public:
    QPsdView(QWidget *parent = nullptr);
    ~QPsdView() override;

    QPsdWidgetTreeItemModel *model() const;
    bool showChecker() const;

public slots:
    void setModel(QPsdWidgetTreeItemModel *model);
    void setItemVisible(quint32 id, bool visible);
    void reset();
    void clearSelection();
    void setShowChecker(bool show);

signals:
    void itemSelected(const QModelIndex &index);
    void modelChanged(QPsdWidgetTreeItemModel *model);
    void showCheckerChanged(bool show);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    class Private;
    QScopedPointer<Private> d;
};

QT_END_NAMESPACE

#endif // QPSDVIEW_H
