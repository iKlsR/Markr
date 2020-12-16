#include "Notification.hpp"

QSharedPointer<Notification> Notification::createNotification()
{
	return QSharedPointer<Notification>(new Notification);
}