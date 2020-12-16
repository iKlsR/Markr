#include "EmptyStateWidget.hpp"

#include <QVBoxLayout>
#include <QVariant>

#include "core/Notification.hpp"

EmptyStateWidget::EmptyStateWidget(QWidget *parent) : QWidget(parent)
{
	QPointer<QLabel> emptyStateLabel(new QLabel(tr("Get started by adding a bookmark"), this));
	emptyStateLabel->setProperty("cssClass", "emptyStateText");
	emptyStateLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	emptyStateLabel->setFont(QFont("Nunito SemiBold", 24));

	QPointer<QLabel> emptyStateLabelAdditional(new QLabel(tr("You can also drag and drop URLs"), this));
	emptyStateLabelAdditional->setProperty("cssClass", "emptyStateText");
	emptyStateLabelAdditional->setProperty("cssClass", "emptyStateTextLesser");
	emptyStateLabelAdditional->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	emptyStateLabelAdditional->setFont(QFont("Nunito SemiBold", 12));

	emptyStateAddButton = new QPushButton(tr("Add New"), this);
	emptyStateAddButton->setProperty("cssClass", "default");
	
	QPointer<QVBoxLayout> emptyStateWidgetLayout(new QVBoxLayout);
	QPointer<QHBoxLayout> emptyStateButtonLayout(new QHBoxLayout);

	QPointer<QWidget> emptyStateButtonWidget(new QWidget(this));
	emptyStateButtonWidget->setLayout(emptyStateButtonLayout);
	emptyStateButtonLayout->addStretch(1);
	emptyStateButtonLayout->addWidget(emptyStateAddButton);
	emptyStateButtonLayout->addStretch(1);
	emptyStateWidgetLayout->addStretch(1);
	emptyStateWidgetLayout->addWidget(emptyStateButtonWidget);
	emptyStateWidgetLayout->addWidget(emptyStateLabel);
	emptyStateWidgetLayout->addWidget(emptyStateLabelAdditional);
	emptyStateWidgetLayout->addStretch(1);
	
	setupActions();

	setMinimumSize(QSize(800, 600));
	setLayout(emptyStateWidgetLayout);
}

EmptyStateWidget::~EmptyStateWidget()
{
}

void EmptyStateWidget::registerNotification(QSharedPointer<Notification> s)
{
	notification = s;
}

void EmptyStateWidget::setupActions()
{
	auto but = connect(emptyStateAddButton, &QPushButton::pressed, this, [&]() {
		emit notification->ShowBookmarkAddWidget();
	});
}
