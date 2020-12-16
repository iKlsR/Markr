#include "Settings.hpp"

#include <QDir>
#include <QApplication>
#include <QDebug>

Settings::Settings()
{
	settingsConfig.configFileName = "markr.config";

	QString path = QDir(QApplication::applicationDirPath()).filePath(settingsConfig.configFileName);
	settings = new QSettings(path, QSettings::IniFormat);

	loadSettings();
}

Settings::~Settings()
{
	// remove this sooner than later
	delete settings;
}

void Settings::loadSettings()
{
	settingsConfig.databasePath		= settings->value("Path/database", "bookmarks.markr").toString();

	settingsConfig.stripProtocol	= settings->value("App/strip_protocol", true).toBool();
	settingsConfig.eyeCandyDisabled = settings->value("App/disable_eye_candy", false).toBool();
	settingsConfig.protectedMode	= settings->value("App/protected_mode", false).toBool();
	settingsConfig.strictValidation = settings->value("App/strict_validation", true).toBool();
	settingsConfig.validateUrls		= settings->value("App/validate_urls", true).toBool();
	settingsConfig.showScrollbar	= settings->value("App/show_scrollbar", true).toBool();
	settingsConfig.textLowercase	= settings->value("App/text_lowercase", false).toBool();
}

void Settings::writeSettings()
{
	settings->beginGroup("Path");
	settings->setValue("database", settingsConfig.databasePath);				// Path to database, if empty, put beside app
	settings->endGroup();

	settings->beginGroup("App");
	settings->setValue("strip_protocol",	settingsConfig.stripProtocol);		// Remove https, http, www etc
	settings->setValue("disable_eye_candy", settingsConfig.eyeCandyDisabled);	// Removes animations, drop shadow
	settings->setValue("protected_mode",	settingsConfig.protectedMode);		// Don't allow deleting records
	settings->setValue("strict_validation", settingsConfig.strictValidation);	// All bookmark fields must be entered
	settings->setValue("validate_urls",		settingsConfig.validateUrls);		// Urls must be valid (match a regex)
	settings->setValue("show_scrollbar",	settingsConfig.showScrollbar);		// Shows the vertical scrollbar
	settings->setValue("text_lowercase",	settingsConfig.textLowercase);		// Renders all text lowercase
	settings->endGroup();
}

void Settings::updateConfig(const SettingsConfig &config)
{
	settingsConfig = config;
	writeSettings();
}

const SettingsConfig &Settings::config() const
{
	return settingsConfig;
}
