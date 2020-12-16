#pragma once

#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>

#include "core/Database.hpp"
#include "core/Settings.hpp"

enum View : int
{
	EmptyState,
	Table,
	Preferences,
	Statistics,
	Undetermined
};

enum RowDisplayMode
{
	WIDE,
	SHRINKING,
	NARROW
};

// Now I could always lay out the correct order of columns in the schema but bah
enum BookmarkColumns : int
{
	Id = 0,
	DateCreated,
	DateUpdated,
	TimesModified,
	Title,
	Url,
	Tags,
	Clicked
};

namespace Markr
{
	Settings *SettingsManager();
	Database *DatabaseInstance();
}

inline void CenterWidgets(QWidget *widget, QWidget *host = 0) {
	if (!host)
		host = widget->parentWidget();

	if (host) {
		auto hostRect = host->geometry();
		widget->move(hostRect.center() - widget->rect().center());
	}
	else {
		QRect screenGeometry = QApplication::desktop()->screenGeometry();
		int x = (screenGeometry.width() - widget->width()) / 2;
		int y = (screenGeometry.height() - widget->height()) / 2;
		widget->move(x, y);
	}
}