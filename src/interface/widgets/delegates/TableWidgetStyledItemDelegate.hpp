#pragma once

#include <QStyledItemDelegate>

class TableWidgetStyledItemDelegate : public QStyledItemDelegate
{

public:
	TableWidgetStyledItemDelegate();
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
	int hoveredRowIndex = -1;

public slots:
	void onHoverIndexChanged(const QModelIndex &index);
	void onLeaveTableEvent(void);
};