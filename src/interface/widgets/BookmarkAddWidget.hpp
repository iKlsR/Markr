#pragma once

#include <QDialog>
#include <QPointer>
#include <QSharedPointer>

#include <QGraphicsDropShadowEffect>

#include "Markr.hpp"
#include "../../core/Bookmark.hpp"

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QShowEvent)
QT_FORWARD_DECLARE_CLASS(QHideEvent)
QT_FORWARD_DECLARE_CLASS(QCloseEvent)

class Notification;

class BookmarkAddWidget : public QDialog
{
	Q_OBJECT

public:
	explicit BookmarkAddWidget(QWidget *parent = Q_NULLPTR);
	virtual ~BookmarkAddWidget();

	void registerNotification(QSharedPointer<Notification> s);
	void fillUrlField(const QString& url);

	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void closeEvent(QCloseEvent *event);

private:
	QPointer<QLabel> formTitle;
	QPointer<QLineEdit> titleField;
	QPointer<QLineEdit> urlField;
	QPointer<QLineEdit> tagsField;
	QPointer<QPushButton> createButton;
	QPointer<QPushButton> cancelButton;

	QGraphicsDropShadowEffect dropShadowEffect;

	QSharedPointer<Notification> notification;

private slots:
	void createNewBookmark();
	void checkValidity(const QString &text);
};