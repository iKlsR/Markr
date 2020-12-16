#include "TableWidgetStyledItemDelegate.hpp"

#include <QPainter>

TableWidgetStyledItemDelegate::TableWidgetStyledItemDelegate() : QStyledItemDelegate()
{

}

void TableWidgetStyledItemDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	QStyleOptionViewItem opt = option;

	if (index.row() == hoveredRowIndex) {
		opt.state |= QStyle::State_MouseOver;
		//painter->fillRect(option.rect, Qt::red);
	}
	else {
		opt.state &= ~QStyle::State_MouseOver;
		//painter->fillRect(option.rect, Qt::transparent);
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

void TableWidgetStyledItemDelegate::onHoverIndexChanged(const QModelIndex &index)
{
	hoveredRowIndex = index.row();
}

void TableWidgetStyledItemDelegate::onLeaveTableEvent() {
	hoveredRowIndex = -1;
}