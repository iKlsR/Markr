#include "Database.hpp"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QSqlQueryModel>
#include <QApplication>

Database::Database(const QString &databasePath)
{
	databaseDriverName = "QSQLITE";
	databaseConnectionName = "MarkrConn";

	bookmarksTableSchema =
		"CREATE TABLE IF NOT EXISTS bookmarks ("
		"    id					INTEGER PRIMARY KEY,"
		"    date_created		DATETIME DEFAULT CURRENT_TIMESTAMP,"
		"    date_updated		DATETIME DEFAULT NULL,"
		"    times_modified		INTEGER DEFAULT 0,"
		"    title				VARCHAR(256) NOT NULL,"
		"    url				VARCHAR(256) NOT NULL,"
		"    tags				VARCHAR(128),"
		"    clicked			INTEGER DEFAULT 0"
		")";

	if (createConnection(databasePath)) {
		QSqlQuery query(databaseHandle);
		query.prepare(bookmarksTableSchema);
		executeAndCheckQuery(query, "CreateBookmarksTable");
	}
}

Database::~Database()
{
	if (databaseHandle.isOpen()) databaseHandle.close();
	QSqlDatabase::removeDatabase(databaseHandle.connectionName());
}

bool Database::createConnection(const QString &databasePath)
{
	if (!QSqlDatabase::isDriverAvailable(databaseDriverName)) {
		qWarning() << QString("Driver: %1").arg(databaseHandle.lastError().text());
		return false;
	}

	databaseHandle = QSqlDatabase::addDatabase(databaseDriverName, databaseConnectionName);
	databaseHandle.setDatabaseName(databasePath);

	if (databaseHandle.isValid()) {
		if (!databaseHandle.open()) {
			qWarning() << QString("Opening: %1") .arg(databaseHandle.lastError().text());
		}

		return databaseHandle.isOpen();
	}
	else {
		qWarning() << QString("Validity: %1") .arg(databaseHandle.lastError().text());
	}

	return false;
}


bool Database::executeAndCheckQuery(QSqlQuery &query, const QString& name)
{
	if (!query.exec()) {
		qWarning() << QString("%1 query failed to execute! %2").arg(name, query.lastError().text());
		return false;
	}

	return true;
}

bool Database::executeAndCheckQueryBatch(QSqlQuery &query, const QString &name)
{
	if (!query.execBatch()) {
		qWarning() << QString("%1 batch query failed to execute! %2").arg(name, query.lastError().text());
		return false;
	}

	return true;
}

bool Database::addBookmark(Bookmark bookmark)
{
	QSqlQuery query(databaseHandle);
	query.prepare(
		"INSERT INTO bookmarks "
		"(date_created, title, url, tags) "
		"VALUES (strftime('%Y-%m-%d'), :title, :url, :tags)"
	);

	query.bindValue(":title", bookmark.title);
	query.bindValue(":url", bookmark.url);
	query.bindValue(":tags", bookmark.tags.join(","));

	return executeAndCheckQuery(query, "AddBookmark");
}

QVector<Bookmark> Database::fetchAllRecords()
{
	QSqlQueryModel model;
	model.setQuery("SELECT * FROM bookmarks", databaseHandle);

	QVector<Bookmark> bookmarks;

	for (int i = 0; i < model.rowCount(); ++i) {
		Bookmark bookmark;

		bookmark.id				= model.record(i).value("id").toInt();
		bookmark.favicon		= model.record(i).value("favicon").toByteArray();
		bookmark.title			= model.record(i).value("title").toString();
		bookmark.tags			= model.record(i).value("tags").toString().split(",");
		bookmark.url			= model.record(i).value("url").toString();
		bookmark.date_created	= model.record(i).value("date_created").toDateTime();
		bookmark.date_updated	= model.record(i).value("date_updated").toDateTime();

	
		bookmarks.append(bookmark);
	}

	return bookmarks;
}

bool Database::updateBookmark(const Bookmark &bookmark)
{
	QSqlQuery query(databaseHandle);
	query.prepare(
		"UPDATE bookmarks "
		"SET title = ?, url = ?, tags = ? "
		"WHERE id = ?"
	);

	query.addBindValue(bookmark.title);
	query.addBindValue(bookmark.url);
	query.addBindValue(bookmark.tags);
	query.addBindValue(bookmark.id);

	return executeAndCheckQuery(query, "UpdateBookmark");
}

bool Database::updateClicked(const unsigned &id)
{
	QSqlQuery query(databaseHandle);
	query.prepare("UPDATE bookmarks SET clicked = clicked + 1 WHERE id = ?");
	query.addBindValue(id);

	return executeAndCheckQuery(query, "UpdateClicked");
}

bool Database::deleteBookmark(unsigned id)
{
	QSqlQuery query(databaseHandle);
	query.prepare("DELETE FROM bookmarks WHERE id = ?");
	query.addBindValue(id);

	return executeAndCheckQuery(query, "DeleteBookmark");
}

bool Database::deleteBookmarks(QVariantList ids)
{
	QString bindingValues;
	// We want to mass delete rows of data so we dynamically build the query string
	for (auto _temp : ids) bindingValues.append("?,");
	bindingValues.chop(1); // Remove the last ,

	QString deleteQuery = QString("DELETE FROM bookmarks WHERE id IN (%1)").arg(bindingValues);

	QSqlQuery query(databaseHandle);
	query.prepare(deleteQuery);
	//*query.addBindValue(ids);
	for (auto id : ids) query.addBindValue(id);

	return executeAndCheckQuery(query, "DeleteBookmarks");
	//*return executeAndCheckQueryBatch(query, "DeleteBookmarks");

	//* Not working for some reason...
}

QSqlDatabase Database::handle()
{
	return databaseHandle;
}
