#pragma once

#include <QSql>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>

#include "Bookmark.hpp"

class Database : public QObject
{
	Q_OBJECT

public:
	Database(const QString &databasePath);
	~Database();

	bool createConnection(const QString &databasePath);
	QSqlDatabase handle();

	bool executeAndCheckQuery(QSqlQuery &query, const QString &name);
	bool executeAndCheckQueryBatch(QSqlQuery &query, const QString &name);

	// Create
	bool addBookmark(Bookmark bookmark);

	// Fetch
	QVector<Bookmark> fetchAllRecords();

	// Update
	bool updateBookmark(const Bookmark &bookmark);
	bool updateClicked(const unsigned &id);

	// Delete
	bool deleteBookmark(unsigned id);
	bool deleteBookmarks(QVariantList ids);

private:
	QSqlDatabase databaseHandle;
	QString databaseDriverName, databaseConnectionName;
	QString bookmarksTableSchema;
};