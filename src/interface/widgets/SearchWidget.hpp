#pragma once

#include <QLineEdit>
#include <QPointer>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QFocusEvent)

class SearchWidget : public QLineEdit
{
	Q_OBJECT

public:
	SearchWidget(QWidget *parent = Q_NULLPTR);
	virtual ~SearchWidget();

	void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
	void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private slots:
	void catchChangedText(const QString &changedText);

signals:
	void searchTermChanged(const QString &searchTerm);

private:
	QTimer searchTimer;
};