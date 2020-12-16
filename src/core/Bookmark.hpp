#pragma once

#include <QDateTime>
#include <QStringList>
#include <QByteArray>

struct Bookmark
{
	unsigned id;
	QString title;
	QStringList tags;
	QString url;
	QDateTime date_created;
	QDateTime date_updated;
	QByteArray favicon;
};