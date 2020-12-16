#pragma once

#include <QTableView>
#include <QSharedPointer>
#include <QMouseEvent>

#include "Markr.hpp"
#include "delegates/TableWidgetStyledItemDelegate.hpp"

class Notification;
class ProxySqlTableModel;

class TableWidget : public QTableView
{
	Q_OBJECT

public:
	TableWidget(ProxySqlTableModel * proxyModel, QWidget *parent = nullptr);
	virtual ~TableWidget();

	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
	void registerNotification(QSharedPointer<Notification> s);

private:
	void openURLsInBrowser(const QModelIndex &modelIndex);

private:
	TableWidgetStyledItemDelegate styledItemDelegate;
	int hoveredIndex = -1;
	QSharedPointer<Notification> notification;

public slots:
	void setRowDisplayMode(RowDisplayMode displayMode);

private slots:
	void onHoverIndexChanged(const QModelIndex &index);
	void onTableDoubleClicked(const QModelIndex &index);
	void onCustomContextMenuRequested(const QPoint &pos);

signals:
	void hoverIndexChanged(QModelIndex rowIndex);
	void leaveTableEvent();
};