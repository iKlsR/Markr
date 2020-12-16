#include "SearchWidget.hpp"

#include <QFocusEvent>
#include <QGraphicsDropShadowEffect>

SearchWidget::SearchWidget(QWidget *parent) : QLineEdit(parent)
{
	setObjectName("SearchWidget");
	setPlaceholderText("Search by keywords, URL fragments or tags...");

	searchTimer.setSingleShot(true);

	connect(this, &QLineEdit::textChanged, this, &SearchWidget::catchChangedText);
}

SearchWidget::~SearchWidget()
{

}

void SearchWidget::focusInEvent(QFocusEvent *event)
{
	QPointer<QGraphicsDropShadowEffect> dropShadowEffect = new QGraphicsDropShadowEffect(this);
	dropShadowEffect->setBlurRadius(20);
	dropShadowEffect->setYOffset(4);
	dropShadowEffect->setXOffset(0);
	dropShadowEffect->setColor(QColor(0, 0, 0, 32));
	setGraphicsEffect(dropShadowEffect);
}

void SearchWidget::focusOutEvent(QFocusEvent *event)
{
	Q_UNUSED(event)
	setGraphicsEffect(Q_NULLPTR);
}

void SearchWidget::catchChangedText(const QString &changedText)
{
	searchTimer.start(100);
	emit searchTermChanged(changedText);
}