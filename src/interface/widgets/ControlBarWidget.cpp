#include "ControlBarWidget.hpp"

#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QToolButton>
#include <QHBoxLayout>

#include "Markr.hpp"
#include "SearchWidget.hpp"
#include "BookmarkAddWidget.hpp"

#include "core/Notification.hpp"

ControlBarWidget::ControlBarWidget(QWidget* parent) : QWidget(parent)
{
	searchWidget = new SearchWidget(this);

	QIcon settings(":/icon/settings.png");
	QIcon theme(":/icon/theme.png");

	settingsButton = new QPushButton(this);
	settingsButton->setIcon(settings);
	settingsButton->setIconSize(QSize(48, 48));
	settingsButton->setProperty("cssClass", "empty");

	//themeSwitcherButton = new QPushButton(this);
	//themeSwitcherButton->setIcon(theme);
	//themeSwitcherButton->setIconSize(QSize(38, 38));
	//themeSwitcherButton->setProperty("cssClass", "empty");

	QMargins margins;
	margins.setBottom(4);

	QPointer<QVBoxLayout> optionsButtonsLayout(new QVBoxLayout);
	optionsButtonsLayout->setMargin(0);
	optionsButtonsLayout->setSpacing(0);
	//optionsButtonsLayout->addWidget(themeSwitcherButton);
	optionsButtonsLayout->addWidget(settingsButton);

	QPointer<QWidget> optionsButtons(new QWidget(this));
	optionsButtons->setLayout(optionsButtonsLayout);

	newBookmarkButton = new QAction(tr("New Bookmark"), this);
	newCollectionButton = new QAction(tr("New Collection"), this);
	newSessionButton = new QAction(tr("New Session"), this);

	QPointer<QMenu> addOptionsMenu(new QMenu(this));
	addOptionsMenu->setObjectName("ActionDropdownMenu");
	addOptionsMenu->addAction(newBookmarkButton);
	addOptionsMenu->addAction(newCollectionButton);
	addOptionsMenu->addAction(newSessionButton);

	bookmarkOptionsButton = new QToolButton(this);
	bookmarkOptionsButton->setPopupMode(QToolButton::MenuButtonPopup);
	bookmarkOptionsButton->setMenu(addOptionsMenu);
	bookmarkOptionsButton->setDefaultAction(newBookmarkButton);

	QPointer<QHBoxLayout> controlBarLayout = new QHBoxLayout;
	controlBarLayout->setContentsMargins(margins);
	//controlBarLayout->addWidget(settingsButton);
	controlBarLayout->addWidget(optionsButtons);
	controlBarLayout->addWidget(searchWidget);
	controlBarLayout->addSpacing(16);
	controlBarLayout->addWidget(bookmarkOptionsButton);

	setupActions();
	setLayout(controlBarLayout);
}

ControlBarWidget::~ControlBarWidget()
{

}

void ControlBarWidget::registerNotification(QSharedPointer<Notification> s)
{
	notification = s;
}

void ControlBarWidget::setTableModelFilter(const QString &searchTerm)
{
	QString filter = QString(
		"url LIKE '%%1%' OR "
		"tags LIKE '%%1%' OR "
		"title LIKE '%%1%'"
	)
	.arg(searchTerm.toLower());

	emit notification->SetBookmarksSearchFltr(filter);
}

void ControlBarWidget::showBookmarkAddWidget(void)
{
	emit notification->ShowBookmarkAddWidget();
}

void ControlBarWidget::showPreferencesWidget(void)
{
	emit notification->ShowPreferencesWidget();
}

void ControlBarWidget::setupActions()
{
	auto src = connect(searchWidget, &SearchWidget::searchTermChanged, this, &ControlBarWidget::setTableModelFilter);
	auto set = connect(settingsButton, &QPushButton::pressed, this, &ControlBarWidget::showPreferencesWidget);
	auto add = connect(newBookmarkButton, &QAction::triggered, this, &ControlBarWidget::showBookmarkAddWidget);
}