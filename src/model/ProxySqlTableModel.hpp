#pragma once

#include <QSqlTableModel>
#include <QVariant>

class ProxySqlTableModel : public QSqlTableModel
{
	Q_OBJECT

public:
	ProxySqlTableModel(QSqlDatabase db = QSqlDatabase(), QObject * parent = Q_NULLPTR);
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};