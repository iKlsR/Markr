#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QString>
#include <QDateTime>
#include <QIcon>
#include <QPixmap>
#include <QStyle>

#include "Markr.hpp"
#include "core/Bookmark.hpp"

class RowWidget : public QWidget
{
	Q_OBJECT

public:
	RowWidget(QWidget *parent = nullptr);
	virtual ~RowWidget();

	void createRowWidgets();
	void setRowData(Bookmark bookmark);
	void setRowDisplayMode(RowDisplayMode displayMode);

private:
	QLabel *bookmarkFavicon;
	QLabel *bookmarkTitle;
	QLabel *bookmarkTags;
	QLabel *bookmarkURL;
	QLabel *bookmarkCreationDate;

	QHBoxLayout *rowWidgetsLayout;
};
