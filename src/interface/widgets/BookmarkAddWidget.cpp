#include "BookmarkAddWidget.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QCloseEvent>

#include "../../core/Notification.hpp"

#include "../../core/BookmarkValidator.hpp"
#include <QDebug>

BookmarkAddWidget::BookmarkAddWidget(QWidget *parent) : QDialog(parent)
{
	setObjectName("BookmarkAddWidget");
	// https://doc.qt.io/qt-5/qt.html#WindowType-enum
	setWindowFlags(windowFlags() | Qt::Popup | Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	setMinimumWidth(700);

	formTitle = new QLabel(tr("Add Bookmark"));
	titleField = new QLineEdit(this);
	titleField->setPlaceholderText(tr("Title"));

	urlField = new QLineEdit(this);
	urlField->setPlaceholderText(tr("URL"));

	if (Markr::SettingsManager()->config().validateUrls) {
		urlField->setPlaceholderText(tr("URL (Strict validation enabled)"));
		// https://forum.qt.io/topic/50933/solved-regex-not-working-in-qt
		// https://regexr.com/4dg27, https://regexr.com/37i6s
		// Regex needs to be escaped to be used in C++
		QRegExp urlRegex;
		urlRegex.setPattern("^(((http|ftp)(s?)\:\/\/)|(www\.))(([a-zA-Z0-9\-\.]+(\.[a-zA-Z0-9\-\.]+)+)|localhost)(\/?)([a-zA-Z0-9\-\.\?\,\'\/\\\+&%\$#_])?([\d\w\.\/\%\+\-\=\&\?\:\\\"\'\,\|\~\;])$");
		urlRegex.indexIn(urlField->text());
		QPointer<QValidator> validator = new QRegExpValidator(urlRegex, this);
		urlField->setValidator(validator);
	}
	
	tagsField = new QLineEdit(this);
	tagsField->setPlaceholderText(tr("Tags (separate with a comma or a space"));

	createButton = new QPushButton(tr("Add"), this);
	createButton->setProperty("cssClass", "default");
	createButton->setDefault(true);
	createButton->setEnabled(!Markr::SettingsManager()->config().strictValidation);

	cancelButton = new QPushButton(tr("Cancel"), this);
	cancelButton->setProperty("cssClass", "defaultLesser");

	dropShadowEffect.setBlurRadius(32);
	dropShadowEffect.setXOffset(0);
	dropShadowEffect.setYOffset(4);
	dropShadowEffect.setColor(QColor(0, 0, 0, 64));

	QPointer<QHBoxLayout> actionButtonsLayout = new QHBoxLayout;
	actionButtonsLayout->setMargin(0);
	actionButtonsLayout->addWidget(cancelButton);
	actionButtonsLayout->addWidget(createButton);
	// +---------------------------------+
	// |  +--------+ +----------------+  |
	// |  | Cancel | |     Create     |  |
	// |  +--------+ +----------------+  |
	// +---------------------------------+
	actionButtonsLayout->setStretch(0, 0);
	actionButtonsLayout->setStretch(1, 1);

	QPointer<QWidget> actionButtons = new QWidget(this);
	actionButtons->setLayout(actionButtonsLayout);

	QPointer<QVBoxLayout> formLayout = new QVBoxLayout;
	formLayout->setSpacing(24);
	formLayout->setMargin(32);
	formLayout->addWidget(formTitle);
	formLayout->addSpacing(24);
	formLayout->addWidget(titleField);
	formLayout->addWidget(urlField);
	formLayout->addWidget(tagsField);
	formLayout->addWidget(actionButtons);
	formLayout->addStretch();

	QPointer<QWidget> container = new QWidget(this);
	container->setObjectName("BookmarkAddWidgetContainer");
	container->setGraphicsEffect(&dropShadowEffect);
	container->setLayout(formLayout);

	QPointer<QVBoxLayout> containerLayout = new QVBoxLayout;
	containerLayout->setMargin(50);
	containerLayout->addWidget(container);

	setLayout(containerLayout);

	connect(cancelButton, &QPushButton::pressed, this, &BookmarkAddWidget::close);
	connect(createButton, &QPushButton::pressed, this, &BookmarkAddWidget::createNewBookmark);

	if (!createButton->isEnabled()) {
		connect(titleField, &QLineEdit::textChanged, this, &BookmarkAddWidget::checkValidity);
		connect(urlField,	&QLineEdit::textChanged, this, &BookmarkAddWidget::checkValidity);
		connect(tagsField,	&QLineEdit::textChanged, this, &BookmarkAddWidget::checkValidity);
	}
}

BookmarkAddWidget::~BookmarkAddWidget()
{

}

void BookmarkAddWidget::registerNotification(QSharedPointer<Notification> s)
{
	notification = s;
}

void BookmarkAddWidget::fillUrlField(const QString &url)
{
	urlField->setText(url);
	urlField->setCursorPosition(0);
}

void BookmarkAddWidget::showEvent(QShowEvent *event)
{
	emit notification->BlurApplicationWidget(true);
	QDialog::showEvent(event);
}

void BookmarkAddWidget::hideEvent(QHideEvent *event)
{
	// Pressing Esc doesn't close the window so we need to do this here if the dialog has focus
	// https://doc.qt.io/archives/qt-4.8/qdialog.html#escape-key
	emit notification->BlurApplicationWidget(false);
	QDialog::hideEvent(event);
}

void BookmarkAddWidget::closeEvent(QCloseEvent *event)
{
	emit notification->BlurApplicationWidget(false);
	QDialog::closeEvent(event);
}

void BookmarkAddWidget::createNewBookmark()
{
	Bookmark bookmark;
	bookmark.title = titleField->text().trimmed();
	bookmark.url = urlField->text().trimmed();
	bookmark.tags = tagsField->text().trimmed().split(",");

	emit notification->CreateNewBookmark(bookmark);

	close();
}

void BookmarkAddWidget::checkValidity(const QString &text)
{
	Q_UNUSED(text)

	bool v = titleField->text().isEmpty() || urlField->text().isEmpty() || tagsField->text().isEmpty();
	createButton->setEnabled(!v);
}
