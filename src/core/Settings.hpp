#pragma once

#include <QPointer>
#include <QSettings>

struct SettingsConfig
{
	QString configFileName;
	QString databasePath;
	bool stripProtocol;
	bool eyeCandyDisabled;
	bool protectedMode;
	bool strictValidation;
	bool validateUrls;
	bool showScrollbar;
	bool textLowercase;
};

class Settings
{
public:
	Settings();
	~Settings();

	void loadSettings();
	void writeSettings();

	void updateConfig(const SettingsConfig &config);

	const SettingsConfig &config() const;

private:
	QPointer<QSettings> settings;
	SettingsConfig settingsConfig;
};