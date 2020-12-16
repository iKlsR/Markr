#pragma once

#include <QObject>
#include <QSharedPointer>

// This class is a catch-all that classes can keep a handle to and trigger events from nested widgets
class Subscriber : public QObject
{
    Q_OBJECT

public:
    static QSharedPointer<Subscriber> createSubscriber();

private:
    Subscriber() = default;
};

typedef Subscriber Sub;