#pragma once

#include <QItemDelegate>

class NotEditableDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	explicit NotEditableDelegate(QObject *parent = nullptr);

protected:
	bool editorEvent(QEvent*, QAbstractItemModel*, const QStyleOptionViewItem&, const QModelIndex&);
	QWidget *createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const;
};