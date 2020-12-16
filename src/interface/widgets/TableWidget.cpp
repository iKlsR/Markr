#include "TableWidget.hpp"

#include <QMenu>
#include <QPointer>
#include <QScrollBar>
#include <QHeaderView>
#include <QDesktopServices>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

#include "core/Notification.hpp"
#include "model/ProxySqlTableModel.hpp"

TableWidget::TableWidget(ProxySqlTableModel *proxyModel, QWidget *parent) : QTableView(parent)
{
	setObjectName("TableWidget");
	
	setAlternatingRowColors(true);
	setShowGrid(false);

	setEditTriggers(QAbstractItemView::NoEditTriggers);	// Explicitly request an edit via context menu later
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	setItemDelegate(&styledItemDelegate);
	setMouseTracking(true);

	setModel(proxyModel);

	hideColumn(BookmarkColumns::Id);
	hideColumn(BookmarkColumns::DateUpdated);
	hideColumn(BookmarkColumns::DateCreated);
	hideColumn(BookmarkColumns::TimesModified);
	hideColumn(BookmarkColumns::Clicked);

	horizontalHeader()->swapSections(BookmarkColumns::Url, BookmarkColumns::Tags);
	horizontalHeader()->moveSection(BookmarkColumns::DateCreated, model()->columnCount() - 1);
	horizontalHeader()->setSectionResizeMode(BookmarkColumns::DateCreated, QHeaderView::ResizeToContents);

	horizontalHeader()->setVisible(false);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	verticalHeader()->setVisible(false);
	verticalHeader()->setDefaultSectionSize(60);

	resizeColumnsToContents();

	connect(this, &TableWidget::doubleClicked, this, &TableWidget::onTableDoubleClicked);
	connect(this, &TableWidget::customContextMenuRequested, this, &TableWidget::onCustomContextMenuRequested);
	connect(this, &TableWidget::hoverIndexChanged, &styledItemDelegate,	&TableWidgetStyledItemDelegate::onHoverIndexChanged);
	connect(this, &TableWidget::leaveTableEvent, &styledItemDelegate, &TableWidgetStyledItemDelegate::onLeaveTableEvent);
}

TableWidget::~TableWidget()
{
}

void TableWidget::registerNotification(QSharedPointer<Notification> s)
{
	notification = s;
}

void TableWidget::mouseMoveEvent(QMouseEvent *event)
{
	QModelIndex index = indexAt(event->pos());
	emit hoverIndexChanged(index);

	QTableView::mouseMoveEvent(event);
}

void TableWidget::leaveEvent(QEvent *event)
{
	emit leaveTableEvent();
	viewport()->update();
}

void TableWidget::onHoverIndexChanged(const QModelIndex &index)
{
	hoveredIndex = index.row();
}

void TableWidget::onTableDoubleClicked(const QModelIndex& index)
{
	if (!index.isValid()) return;
	int row = index.row();

	QModelIndexList selectedRow = selectionModel()->selectedRows();
	QModelIndex selection = selectedRow.at(0);
	openURLsInBrowser(selection);
}

#include <QDebug>
void TableWidget::openURLsInBrowser(const QModelIndex &modelIndex)
{
	QVariant url = modelIndex.sibling(modelIndex.row(), BookmarkColumns::Url).data();
	if (QDesktopServices::openUrl(QUrl(url.toString()))) {
		int id = modelIndex.sibling(modelIndex.row(), modelIndex.column()).data().toInt();
		Markr::DatabaseInstance()->updateClicked(id);
	}
}

void TableWidget::onCustomContextMenuRequested(const QPoint& pos)
{
	if (rowAt(pos.y()) >= 0) {
		QModelIndexList selection = selectionModel()->selectedRows();

		QMenu rowContextMenu(this);
		rowContextMenu.setObjectName("RowContextMenu");
		rowContextMenu.setWindowFlags(rowContextMenu.windowFlags() | Qt::NoDropShadowWindowHint);

		//QGraphicsDropShadowEffect dropShadowEffect;
		//dropShadowEffect.setBlurRadius(20);
		//dropShadowEffect.setYOffset(4);
		//dropShadowEffect.setXOffset(0);
		//dropShadowEffect.setColor(QColor(0, 0, 0, 32));

		//rowContextMenu.setGraphicsEffect(&dropShadowEffect);

		QPointer<QAction> openMenuAction;
		QPointer<QAction> editMenuAction;
		QPointer<QAction> deleteMenuAction;

		openMenuAction = rowContextMenu.addAction(tr("Open in browser"));

		if (selection.count() == 1) {
			editMenuAction = rowContextMenu.addAction(tr("Edit"));
			deleteMenuAction = rowContextMenu.addAction(tr("Delete"));
		}
		else if (selection.count() > 1) {
			deleteMenuAction = rowContextMenu.addAction(tr("Delete %1 bookmarks").arg(selection.count()));
		}

		if (Markr::SettingsManager()->config().protectedMode) {
			rowContextMenu.removeAction(editMenuAction);
			rowContextMenu.removeAction(deleteMenuAction);
		}

		QPointer<QAction> actionPressed = rowContextMenu.exec(viewport()->mapToGlobal(pos));

		QVariantList idsToDelete;

		if (actionPressed == openMenuAction) {
			for (auto item : selection) {
				openURLsInBrowser(item);
			}
		}

		if (actionPressed == deleteMenuAction) {
			for (auto item : selection) {
				// Gets the value of the first cell (ID)
				QVariant value = item.sibling(item.row(), item.column()).data();
				idsToDelete << value;
			}

			emit notification->DeleteBookmarks(idsToDelete);
		}
	}
}

void TableWidget::setRowDisplayMode(RowDisplayMode displayMode)
{
	//QGraphicsOpacityEffect *fade_effect = new QGraphicsOpacityEffect(this);
	//setGraphicsEffect(fade_effect);

	//QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
	//animation->setEasingCurve(QEasingCurve::InOutQuad);
	//animation->setDuration(500);
	//animation->setStartValue(0.01);
	//animation->setEndValue(1.0);
	//animation->start(QPropertyAnimation::DeleteWhenStopped);

	switch (displayMode) {
		case WIDE: {
			showColumn(BookmarkColumns::Title);
			showColumn(BookmarkColumns::Tags);
			showColumn(BookmarkColumns::Url);
			break;
		}

		case SHRINKING: {
			showColumn(BookmarkColumns::Title);
			showColumn(BookmarkColumns::Tags);
			hideColumn(BookmarkColumns::Url);
			break;
		}

		case NARROW: {
			showColumn(BookmarkColumns::Title);
			hideColumn(BookmarkColumns::Tags);
			hideColumn(BookmarkColumns::Url);

			resizeColumnsToContents();
			break;
		}

		default: break;
	}
}