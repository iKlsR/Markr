// ((options))
// super minimal mode (only one col), pick which one
// open theme file
// export all urls to json
// import urls from json (see format) - reject duplicates

#include "PreferencesWidget.hpp"

#include <QFormLayout>

#include "core/Notification.hpp"
#include "core/Settings.hpp"

PreferencesWidget::PreferencesWidget(QWidget *parent) : QWidget(parent)
{
	setObjectName("PreferencesWidget");

	createWidgets();
	setupActions();
}

PreferencesWidget::~PreferencesWidget()
{

}

void PreferencesWidget::registerNotification(QSharedPointer<Notification> s)
{
	notification = s;
}

void PreferencesWidget::createWidgets()
{
	settingsConfig = Markr::SettingsManager()->config();

	stripProtocolCheckbox = new QCheckBox(this);
	stripProtocolCheckbox->setChecked(settingsConfig.stripProtocol);
	stripProtocolCheckbox->setCursor(Qt::PointingHandCursor);
	stripProtocolCheckbox->setToolTip("Hides https, http, www and other prefixes from URLs");

	disableEyecandyCheckbox = new QCheckBox(this);
	disableEyecandyCheckbox->setChecked(settingsConfig.eyeCandyDisabled);
	disableEyecandyCheckbox->setCursor(Qt::PointingHandCursor);
	disableEyecandyCheckbox->setToolTip("Disable animations, drop shadows, blurs");

	protectedModeCheckbox = new QCheckBox(this);
	protectedModeCheckbox->setChecked(settingsConfig.protectedMode);
	protectedModeCheckbox->setCursor(Qt::PointingHandCursor);
	protectedModeCheckbox->setToolTip("Don't allow deleting records");

	strictValidationCheckbox = new QCheckBox(this);
	strictValidationCheckbox->setChecked(settingsConfig.strictValidation);
	strictValidationCheckbox->setCursor(Qt::PointingHandCursor);
	strictValidationCheckbox->setToolTip("All fields must be entered for new bookmarks");

	validateURLsCheckbox = new QCheckBox(this);
	validateURLsCheckbox->setChecked(settingsConfig.validateUrls);
	validateURLsCheckbox->setCursor(Qt::PointingHandCursor);
	validateURLsCheckbox->setToolTip("URLs must be properly formatted (pass a regex check)");

	scrollbarCheckbox = new QCheckBox(this);
	scrollbarCheckbox->setChecked(settingsConfig.showScrollbar);
	scrollbarCheckbox->setCursor(Qt::PointingHandCursor);
	scrollbarCheckbox->setToolTip("Hide the main scrollbar");

	textLowercaseCheckbox = new QCheckBox(this);
	textLowercaseCheckbox->setChecked(settingsConfig.textLowercase);
	textLowercaseCheckbox->setCursor(Qt::PointingHandCursor);
	textLowercaseCheckbox->setToolTip("Render all text using lowercase");

	databasePath = new QLineEdit(this);
	databasePath->setReadOnly(true);
	databasePath->setPlaceholderText("Defaults to the location of this executable");
	databasePath->setText(settingsConfig.databasePath);
	databasePath->setCursorPosition(0);
	databasePath->setToolTip("Path to the database (click to change)");

	QPointer<QVBoxLayout> mainLayout(new QVBoxLayout);
	mainLayout->setAlignment(Qt::AlignCenter);
	
	QPointer<QWidget> mainWidget(new QWidget(this));
	mainWidget->setLayout(mainLayout);

	QPointer<QFormLayout> formLayout(new QFormLayout);
	formLayout->setHorizontalSpacing(64);
	formLayout->setVerticalSpacing(24);
	formLayout->setMargin(64);
	formLayout->addRow(tr("Strip protocol from URLs"), stripProtocolCheckbox);
	formLayout->addRow(tr("Disable eye candy"), disableEyecandyCheckbox);
	formLayout->addRow(tr("Protected mode"), protectedModeCheckbox);
	formLayout->addRow(tr("Strict validation"), strictValidationCheckbox);
	formLayout->addRow(tr("Validate URLs"), validateURLsCheckbox);
	formLayout->addRow(tr("Show scrollbar"), scrollbarCheckbox);
	formLayout->addRow(tr("Render all text lowercase"), textLowercaseCheckbox);
	formLayout->addRow(tr("Database path"), databasePath);

	QPointer<QWidget> formWidget(new QWidget(mainWidget));
	formWidget->setMaximumWidth(1024);
	formWidget->setObjectName("MainWidget");
	formWidget->setLayout(formLayout);
	formWidget->setFont(QFont("Nunito SemiBold", 16));

	returnButton = new QPushButton("save and close", this);
	returnButton->setCursor(Qt::PointingHandCursor);
	returnButton->setObjectName("ReturnButton");
	returnButton->setProperty("cssClass", "defaultLesser");

	mainLayout->addWidget(returnButton, 0, Qt::AlignRight);
	mainLayout->addSpacing(24);
	mainLayout->addWidget(formWidget);

	setLayout(mainLayout);
}

void PreferencesWidget::setupActions()
{
	auto ret = connect(returnButton, &QPushButton::pressed, this, &PreferencesWidget::closeView);
}

void PreferencesWidget::closeView(void)
{
	settingsConfig.stripProtocol	= stripProtocolCheckbox->isChecked();
	settingsConfig.eyeCandyDisabled = disableEyecandyCheckbox->isChecked();
	settingsConfig.protectedMode	= protectedModeCheckbox->isChecked();
	settingsConfig.strictValidation = strictValidationCheckbox->isChecked();
	settingsConfig.validateUrls		= validateURLsCheckbox->isChecked();
	settingsConfig.showScrollbar	= scrollbarCheckbox->isChecked();
	settingsConfig.textLowercase	= textLowercaseCheckbox->isChecked();

	Markr::SettingsManager()->updateConfig(settingsConfig);

	emit notification->ChangeView(View::Undetermined);
}
