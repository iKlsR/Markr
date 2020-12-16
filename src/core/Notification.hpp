#pragma once

#include <QObject>
#include <QSharedPointer>

#include "Bookmark.hpp"
#include "Markr.hpp"

// This class is a catch-all that classes can keep a handle to and trigger events from nested widgets
class Notification : public QObject
{
	Q_OBJECT

public:
	static QSharedPointer<Notification> createNotification();

private:
	Notification() = default;

signals:
	void CreateNewBookmark(const Bookmark &bookmark);
	void UpdateBookmark(const Bookmark &bookmark);
	void DeleteBookmark(unsigned id);
	void DeleteBookmarks(QVariantList ids);
	void SetBookmarksSearchFltr(const QString &filter);
	void BlurApplicationWidget(bool blurred);
	void ShowBookmarkAddWidget(const QString &urlToPrefill = QString());
	void ShowPreferencesWidget();
	void ChangeView(View view);
};

typedef Notification Notify;