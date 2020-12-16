#pragma once

#include <QWidget>
#include <QPointer>
#include <QSharedPointer>
#include <QPushButton>
#include <QLabel>

class Notification;

class EmptyStateWidget : public QWidget
{
	Q_OBJECT

public:
	explicit EmptyStateWidget(QWidget *parent = Q_NULLPTR);
	virtual ~EmptyStateWidget();

	void registerNotification(QSharedPointer<Notification> s);

private:
	void setupActions();

private:
	QSharedPointer<Notification> notification;
	QPointer<QPushButton> emptyStateAddButton;
};