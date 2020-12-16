#pragma once

#include <QWidget>
#include <QPointer>
#include <QSharedPointer>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QVariant>

class Notification;

#include "Markr.hpp"

class PreferencesWidget : public QWidget
{
	Q_OBJECT

public:
	explicit PreferencesWidget(QWidget *parent = Q_NULLPTR);
	virtual ~PreferencesWidget();

	void registerNotification(QSharedPointer<Notification> s);

private:
	void createWidgets();
	void setupActions();

private:
	QSharedPointer<Notification> notification;
	QPointer<QCheckBox> stripProtocolCheckbox;
	QPointer<QCheckBox> disableEyecandyCheckbox;
	QPointer<QCheckBox> protectedModeCheckbox;
	QPointer<QCheckBox> strictValidationCheckbox;
	QPointer<QCheckBox> validateURLsCheckbox;
	QPointer<QCheckBox> scrollbarCheckbox;
	QPointer<QCheckBox> textLowercaseCheckbox;
	QPointer<QLineEdit> databasePath;
	QPointer<QPushButton> returnButton;

	SettingsConfig settingsConfig;

private slots:
	void closeView(void);
};