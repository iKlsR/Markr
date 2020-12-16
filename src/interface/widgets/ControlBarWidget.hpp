#pragma once

#include <QPointer>
#include <QPushButton>
#include <QSharedPointer>

QT_FORWARD_DECLARE_CLASS(QToolButton)
QT_FORWARD_DECLARE_CLASS(QAction)

class SearchWidget;
class Notification;

class ControlBarWidget : public QWidget
{
	Q_OBJECT

public:
	ControlBarWidget(QWidget* parent = nullptr);
	virtual ~ControlBarWidget();

	void registerNotification(QSharedPointer<Notification> s);

private:
	void setupActions();

private:
	QPointer<SearchWidget> searchWidget;
	QPointer<QToolButton> bookmarkOptionsButton;
	QPointer<QPushButton> settingsButton;
	QPointer<QPushButton> themeSwitcherButton;
	QSharedPointer<Notification> notification;

	QPointer<QAction> newBookmarkButton;
	QPointer<QAction> newCollectionButton;
	QPointer<QAction> newSessionButton;

public slots:
	void setTableModelFilter(const QString &searchTerm);
	void showBookmarkAddWidget(void);
	void showPreferencesWidget(void);
};