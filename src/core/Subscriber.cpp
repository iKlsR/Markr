#include "Subscriber.hpp"

QSharedPointer<Subscriber> Subscriber::createSubscriber()
{
    return QSharedPointer<Subscriber>(new Subscriber);
}