#include "BookmarkValidator.hpp"

#include "Markr.hpp"
#include "Settings.hpp"

#include <QDebug>

BookmarkValidator::BookmarkValidator()
{

}

BookmarkValidator::~BookmarkValidator()
{

}

QString BookmarkValidator::StripProtocolFromURL(const QString &url)
{
	// Want consistent lines and doesn't matter in the long run
	QString modifiedURL = url.toLower();

	if (Markr::SettingsManager()->config().stripProtocol) {
		// https://regexr.com/4dfuc
		return modifiedURL.replace(QRegExp("^(?:https?:\/\/)?(?:www\.)?"), "");
	}

	return modifiedURL;
}

QString BookmarkValidator::ModifyRenderedText(const QString &url)
{
	return Markr::SettingsManager()->config().textLowercase ? url.toLower() : url;
}