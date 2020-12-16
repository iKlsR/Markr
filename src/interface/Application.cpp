#include "Application.hpp"

#include <QDebug>
#include <QLineEdit>
#include <QListWidget>
#include <QFontDatabase>
#include <QApplication>
#include <QListWidget>
#include <QStackedWidget>
#include <QFile>
#include <QDrag>
#include <QTextStream>
#include <QHeaderView>
#include <QResizeEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMouseEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QtGui\QStandardItem>
#include <QtWidgets\QGraphicsDropShadowEffect>

#include "widgets/RowWidget.hpp"
#include "widgets/ControlBarWidget.hpp"
#include "widgets/TableWidget.hpp"
#include "core/Notification.hpp"
#include "core/Subscriber.hpp"
#include "model/ProxySqlTableModel.hpp"
#include "core/Settings.hpp"
#include "widgets/BookmarkAddWidget.hpp"
#include "widgets/EmptyStateWidget.hpp"
#include "widgets/PreferencesWidget.hpp"

//namespace Markr {
//	Q_GLOBAL_STATIC(Settings, SettingsManager)
//	Q_GLOBAL_STATIC_WITH_ARGS(Database, DatabaseInstance, (SettingsManager->config().databasePath))
//};

Application::Application()
{
	setObjectName("Application");

	mainWidget = new QStackedWidget(this);

	notification = Notification::createNotification();
	subscriber = Subscriber::createSubscriber();

	database = Markr::DatabaseInstance();

	int id = QFontDatabase::addApplicationFont(":/Nunito-SemiBold.ttf");
	QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	QFont monospace(family);

	QApplication::setFont(monospace);
	setWindowIcon(QIcon(":/icon.ico"));

	//list = new QListWidget(this);
	//list->setAlternatingRowColors(true);

	//for (const auto bookmark : database->fetchAllRecords()) {
	//	QListWidgetItem * lw = new QListWidgetItem(list);
	//	auto row = new RowWidget(this);
	//	row->setRowData(bookmark);
	//	lw->setSizeHint(row->minimumSizeHint() + QSize(0, 8));
	//	calculateApplicationWindowMinWidth(row->minimumSizeHint().width());
	//	list->setItemWidget(lw, row);
	//}

	setMouseTracking(true);
	installEventFilter(this);
	setAcceptDrops(true);

	bookmarksTableModel = new ProxySqlTableModel(database->handle());
	bookmarksTableModel->setTable("bookmarks");
	bookmarksTableModel->select();

	emptyStateWidget = new EmptyStateWidget(mainWidget);
	emptyStateWidget->registerNotification(notification);

	preferencesWidget = new PreferencesWidget(mainWidget);
	preferencesWidget->registerNotification(notification);

	bookmarksTableWidget = new TableWidget(bookmarksTableModel, mainWidget);
	bookmarksTableWidget->registerNotification(notification);
	bookmarksTableWidget->show();

	controlBarWidget = new ControlBarWidget(mainWidget);
	controlBarWidget->registerNotification(notification);

	QPointer<QVBoxLayout> centralWidgetLayout = new QVBoxLayout;
	QPointer<QWidget> viewingStateWidget = new QWidget(this);
	viewingStateWidget->setLayout(centralWidgetLayout);
	centralWidgetLayout->addWidget(controlBarWidget);
	centralWidgetLayout->addWidget(bookmarksTableWidget);

	mainWidget->addWidget(emptyStateWidget);
	mainWidget->addWidget(viewingStateWidget);
	mainWidget->addWidget(preferencesWidget);

	setCentralWidget(mainWidget);

	QFile styleSheet;
	styleSheet.setFileName(QString::fromUtf8(":/style.qss"));

	if (styleSheet.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream fileContents(&styleSheet);
		setStyleSheet(fileContents.readAll());
	}

	setupActions();
	changeView(View::Undetermined);
}

Application::~Application()
{
}

void Application::windowWidthChanged(RowDisplayMode mode)
{
	//for (int i = 0; i < list->count(); ++i) {
	//	QListWidgetItem *item = list->item(i);
	//	static_cast<RowWidget *>(list->itemWidget(item))->setRowDisplayMode(mode);
	//}
}

void Application::blurApplicationWindow(bool blurred)
{
	if (!Markr::SettingsManager()->config().eyeCandyDisabled) {
		if (blurred) {
			QPointer<QGraphicsBlurEffect> windowBlurEffect = new QGraphicsBlurEffect(this);
			windowBlurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
			setGraphicsEffect(windowBlurEffect);
		}
		else {
			setGraphicsEffect(Q_NULLPTR);
		}
	}
}

void Application::showBookmarkAddWidget(const QString &url)
{
	const QPointer<BookmarkAddWidget> bookmarkAddWidget(new BookmarkAddWidget(this));
	bookmarkAddWidget->fillUrlField(url);
	bookmarkAddWidget->registerNotification(notification);
	bookmarkAddWidget->show();
}

void Application::showPreferencesWidget(void)
{
	changeView(View::Preferences);
}

void Application::CreateNewBookmark(const Bookmark &bookmark)
{
	database->addBookmark(bookmark);
	updateView();
}

void Application::DeleteBookmarks(QVariantList idsToDelete)
{
	database->deleteBookmarks(idsToDelete);
	updateView();
}

void Application::SetBookmarksSearchFilter(const QString &filter)
{
	bookmarksTableModel->setFilter(filter);
}

void Application::resizeEvent(QResizeEvent *event)
{
	// This is a good way to check if this is the very FIRST startup resize event called
	// if (event->oldSize() != QSize(-1, -1))

	int width = event->size().width();
	// 1920 + Show everything
	// 1600 - title and tags
	// 1280 - title and favicon only
	if (width > 1280 && width <= 1600) {
		bookmarksTableWidget->setRowDisplayMode(RowDisplayMode::SHRINKING);
	}
	else if (width <= 1280) {
		bookmarksTableWidget->setRowDisplayMode(RowDisplayMode::NARROW);
	}
	else {
		bookmarksTableWidget->setRowDisplayMode(RowDisplayMode::WIDE);
	}
	
	QMainWindow::resizeEvent(event);
}

void Application::dragEnterEvent(QDragEnterEvent* evt)
{
	if (evt->mimeData()->hasUrls()) {
		evt->acceptProposedAction();
	}
}

void Application::dragMoveEvent(QDragMoveEvent* event)
{
	event->accept();
}

void Application::dropEvent(QDropEvent *evt)
{
	auto url = evt->mimeData()->urls().last();
	evt->acceptProposedAction();

	emit notification->ShowBookmarkAddWidget(url.toString());
}

void Application::calculateApplicationWindowMinWidth(int oldWidth)
{
	if (oldWidth > minimumWidth()) setMinimumWidth(oldWidth + /* add some padding */ 32);
}

void Application::changeView(View view)
{
	switch (view) {
		case View::Undetermined: {
			bookmarksTableModel->rowCount()
				? mainWidget->setCurrentIndex(View::Table)
				: mainWidget->setCurrentIndex(View::EmptyState);
			break;
		}

		case View::EmptyState: {
			mainWidget->setCurrentIndex(View::EmptyState);
			break;
		}

		case View::Preferences: {
			mainWidget->setCurrentIndex(View::Preferences);
			break;
		}

		case View::Table: 
		default: mainWidget->setCurrentIndex(View::Table);
	}

	if (Markr::SettingsManager()->config().showScrollbar) {
		bookmarksTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
	else {
		bookmarksTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
}

void Application::updateView()
{
	bookmarksTableModel->select();
	changeView(View::Undetermined);
}

void Application::setupActions()
{
	connect(notification.data(), &Notification::CreateNewBookmark,		this, &Application::CreateNewBookmark);
	connect(notification.data(), &Notification::DeleteBookmarks,		this, &Application::DeleteBookmarks);
	connect(notification.data(), &Notification::SetBookmarksSearchFltr, this, &Application::SetBookmarksSearchFilter);
	connect(notification.data(), &Notification::ShowBookmarkAddWidget,	this, &Application::showBookmarkAddWidget);
	connect(notification.data(), &Notification::BlurApplicationWidget,	this, &Application::blurApplicationWindow);
	connect(notification.data(), &Notification::ShowPreferencesWidget,	this, &Application::showPreferencesWidget);
	connect(notification.data(), &Notification::ChangeView,				this, &Application::changeView);
}

bool Application::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		auto evt = static_cast<QMouseEvent*>(event);

		if (evt->button() == Qt::LeftButton) dragStartPosition = evt->pos();

		Application::mousePressEvent(evt);
	}

	if (event->type() == QEvent::MouseMove) {
		auto evt = static_cast<QMouseEvent*>(event);

		int distance = (evt->pos() - dragStartPosition).manhattanLength();
		if (distance >= QApplication::startDragDistance()) {
			auto drag = QPointer<QDrag>(new QDrag(this));
			drag->exec();
		}

		QScopedPointer<QDrag> drag(new QDrag(this));
		QMimeData* mimeData = new QMimeData;
		drag->setMimeData(mimeData);

		Qt::DropAction dropAction = drag->exec(Qt::CopyAction);

		Application::mouseMoveEvent(evt);
	}


	return QObject::eventFilter(object, event);
}
