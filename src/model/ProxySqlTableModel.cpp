#include "ProxySqlTableModel.hpp"

#include <QAbstractTableModel>

#include "Markr.hpp"
#include "core/BookmarkValidator.hpp"

ProxySqlTableModel::ProxySqlTableModel(QSqlDatabase db, QObject* parent)
	: QSqlTableModel(parent, db)
{
}

// https://doc.qt.io/qt-5/modelview.html#2-a-simple-model-view-application
// https://stackoverflow.com/a/4959104/996468
QVariant ProxySqlTableModel::data(const QModelIndex &idx, int role) const
{
	QVariant value = QSqlTableModel::data(idx, role);

	if (role == Qt::TextAlignmentRole) {
		switch (idx.column()) {
			case BookmarkColumns::Url:
				// For some reason, the right align shifts the text up on its own
				return Qt::AlignRight | Qt::AlignVCenter;
			break;
			case BookmarkColumns::Tags:
				return Qt::AlignCenter;
			break;
		}
	}

	if (role == Qt::DisplayRole) {
		switch (idx.column()) {
			case BookmarkColumns::Url:
				value = BookmarkValidator::StripProtocolFromURL(value.toString());
			break;
		}

		value = BookmarkValidator::ModifyRenderedText(value.toString());
	}

	return value;
}
