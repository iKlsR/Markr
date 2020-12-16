#pragma once

#include <QValidator>
#include <QRegExp>

class BookmarkValidator
{
public:
	explicit BookmarkValidator();
	~BookmarkValidator();

	static QString StripProtocolFromURL(const QString &url);
	static QString ModifyRenderedText(const QString &url);
};