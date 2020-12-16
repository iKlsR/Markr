#pragma once

#include <QPointer>
#include <QMainWindow>
#include <QSharedPointer>

#include "Markr.hpp"
#include "core/Database.hpp"

QT_FORWARD_DECLARE_CLASS(QResizeEvent)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)

QT_FORWARD_DECLARE_CLASS(QDragEnterEvent)
QT_FORWARD_DECLARE_CLASS(QDragMoveEvent)
QT_FORWARD_DECLARE_CLASS(QDropEvent)

class Database;
class Subscriber;
class Notification;
class TableWidget;
class QListWidget;
class ControlBarWidget;
class EmptyStateWidget;
class PreferencesWidget;
class ProxySqlTableModel;

class Application : public QMainWindow
{
	Q_OBJECT

public:
	Application();
	virtual ~Application();

	void resizeEvent(QResizeEvent *event);

	QSharedPointer<Notification> notification;
	QSharedPointer<Subscriber> subscriber;

	void dragEnterEvent(QDragEnterEvent* evt);
	void dragMoveEvent(QDragMoveEvent* evt);
	void dropEvent(QDropEvent* evt);

private:
	void calculateApplicationWindowMinWidth(int oldWidth);
	void changeView(View view = View::Undetermined);
	void updateView();
	void setupActions();
	bool eventFilter(QObject* object, QEvent* event);

private:
	QPointer<QListWidget> list;
	QPointer<QStackedWidget> mainWidget;
	QPointer<ControlBarWidget> controlBarWidget;
	QPointer<TableWidget> bookmarksTableWidget;
	QPointer<EmptyStateWidget> emptyStateWidget;
	QPointer<PreferencesWidget> preferencesWidget;
	QPointer<ProxySqlTableModel> bookmarksTableModel;
	QPointer<Database> database;
	QPoint dragStartPosition;

private slots:
	void windowWidthChanged(RowDisplayMode rowDisplayMode);
	void blurApplicationWindow(bool blurred);
	void showBookmarkAddWidget(const QString& url);
	void showPreferencesWidget(void);
	void CreateNewBookmark(const Bookmark &bookmark);
	void DeleteBookmarks(QVariantList idsToDelete);
	void SetBookmarksSearchFilter(const QString &filter);

signals:
	void applicationWindowWidthChanged(RowDisplayMode rowDisplayMode);
};