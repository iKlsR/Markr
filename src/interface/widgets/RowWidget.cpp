#include "RowWidget.hpp"
#include <QtWidgets\QGraphicsOpacityEffect>
#include <QtCore\QPropertyAnimation>

RowWidget::RowWidget(QWidget *parent) : QWidget(parent)
{
	setObjectName("ListWidget");
	setAccessibleName("ListWidgetAccessible");

	createRowWidgets();
}

RowWidget::~RowWidget()
{

}

void RowWidget::createRowWidgets()
{
	QSizePolicy bookmarkTitleSizePolicy;
	bookmarkTitleSizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
	bookmarkTitleSizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);

	QSizePolicy bookmarkCreationDateSizePolicy;
	bookmarkCreationDateSizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
	bookmarkCreationDateSizePolicy.setVerticalPolicy(QSizePolicy::Preferred);

	bookmarkFavicon = new QLabel;
	bookmarkFavicon->setAlignment(Qt::AlignLeft);

	bookmarkTitle = new QLabel;
	bookmarkTitle->setSizePolicy(bookmarkTitleSizePolicy);
	bookmarkTitle->setAlignment(Qt::AlignLeft);

	bookmarkTags = new QLabel;
	bookmarkTags->setAlignment(Qt::AlignCenter);

	bookmarkURL = new QLabel;
	bookmarkURL->setAlignment(Qt::AlignRight);
	
	bookmarkCreationDate = new QLabel(QDateTime::currentDateTime().toString());
	bookmarkCreationDate->setSizePolicy(bookmarkCreationDateSizePolicy);

	rowWidgetsLayout = new QHBoxLayout;
	//rowWidgetsLayout->setSpacing(32);
	//rowWidgetsLayout->setMargin(16);
	rowWidgetsLayout->addWidget(bookmarkFavicon);
	rowWidgetsLayout->addSpacing(4);
	rowWidgetsLayout->addWidget(bookmarkTitle);
	rowWidgetsLayout->addWidget(bookmarkTags);
	rowWidgetsLayout->addWidget(bookmarkURL);
	rowWidgetsLayout->addSpacing(16);
	rowWidgetsLayout->addWidget(bookmarkCreationDate);

	setLayout(rowWidgetsLayout);
}

void RowWidget::setRowData(Bookmark bookmark)
{
	bookmarkTitle->setText(bookmark.title);
	bookmarkTags->setText(bookmark.tags.join(", "));
	bookmarkURL->setText(bookmark.url);
	bookmarkCreationDate->setText(bookmark.date_created.toString());

	QPixmap pixmap;
	if (!pixmap.loadFromData(bookmark.favicon, "ICO")) {
		QIcon icon = style()->standardIcon(QStyle::SP_FileIcon);
		pixmap = icon.pixmap(QSize(24, 24));
	}

	bookmarkFavicon->setPixmap(pixmap);
	bookmarkFavicon->setMaximumWidth(24);
}

void RowWidget::setRowDisplayMode(RowDisplayMode displayMode)
{
	//QGraphicsOpacityEffect *fade_effect = new QGraphicsOpacityEffect(this);
	//bookmarkURL->setGraphicsEffect(fade_effect);
	//bookmarkTags->setGraphicsEffect(fade_effect);
	//bookmarkCreationDate->setGraphicsEffect(fade_effect);

	//QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
	//animation->setEasingCurve(QEasingCurve::InOutQuad);
	//animation->setDuration(500);
	//animation->setStartValue(0.01);
	//animation->setEndValue(1.0);
	//animation->start(QPropertyAnimation::DeleteWhenStopped);

	switch (displayMode)
	{
	case WIDE: {
		bookmarkURL->setVisible(true);
		bookmarkTags->setVisible(true);
		bookmarkCreationDate->setVisible(true);
		break;
	}
	case SHRINKING:
		bookmarkTags->setVisible(true);
		bookmarkURL->setVisible(false);
		bookmarkCreationDate->setVisible(false);
		break;
	case NARROW:
		bookmarkTags->setVisible(false);
		bookmarkURL->setVisible(false);
		bookmarkCreationDate->setVisible(false);
		break;
	default:
		break;
	}

	bookmarkTags->hide();

}
