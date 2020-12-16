#include "Markr.hpp"

// QSqlDatabase: an instance of QCoreApplication is required for loading driver plugins
// You're not allowed to load sql plugins on the global level.
// Just don't try to make it "static inside class", but make it "static inside static method",
// like in a typical singleton pattern. Sort of...
// ACTUALLY IGNORE THIS, it's apparently unsafe in a sense -- https://stackoverflow.com/a/20645276/996468
//Database &Markr::createDatabaseHandle()
//{
//	static Database database;
//	return database;
//}

namespace Markr
{
	Q_GLOBAL_STATIC(Settings, SettingsManagerBase)
	Q_GLOBAL_STATIC_WITH_ARGS(Database, DatabaseInstanceBase, (SettingsManagerBase->config().databasePath))

	Settings *SettingsManager() {
		return SettingsManagerBase;
	}

	Database *DatabaseInstance() {
		return DatabaseInstanceBase;
	}
}