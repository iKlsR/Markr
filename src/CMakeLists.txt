# ┌──────────────────────────────────────────────────────────────────┐
# │  Projects Settings                                               │
# └──────────────────────────────────────────────────────────────────┘
cmake_minimum_required (VERSION 3.8)
project (Markr)

# ┌──────────────────────────────────────────────────────────────────┐
# │  C++                                                             │
# └──────────────────────────────────────────────────────────────────┘

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

# ┌──────────────────────────────────────────────────────────────────┐
# │  Qt                                                              │
# └──────────────────────────────────────────────────────────────────┘

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
find_package (Qt5 REQUIRED COMPONENTS Widgets Sql)

# set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER AutoMocFolder)

# ┌──────────────────────────────────────────────────────────────────┐
# │  Source files                                                    │
# └──────────────────────────────────────────────────────────────────┘

set (SRC
	"main.cpp"
	"Markr.cpp"
	"core/Database.cpp"
	"core/Settings.cpp"
	"core/Subscriber.cpp"
	"core/Notification.cpp"
	"core/BookmarkValidator.cpp"
	"model/ProxySqlTableModel.cpp"
	"interface/Application.cpp"
	"interface/widgets/RowWidget.cpp"
	"interface/widgets/TableWidget.cpp"
	"interface/widgets/SearchWidget.cpp"
	"interface/widgets/EmptyStateWidget.cpp"
	"interface/widgets/ControlBarWidget.cpp"
	"interface/widgets/BookmarkAddWidget.cpp"
	"interface/widgets/PreferencesWidget.cpp"
	"interface/widgets/delegates/TableWidgetStyledItemDelegate.cpp")

set (QRC
	${CMAKE_SOURCE_DIR}/src/res/resources.qrc
	${CMAKE_SOURCE_DIR}/src/res/icon.rc)

# ┌──────────────────────────────────────────────────────────────────┐
# │  Applications  (entry points)                                    │
# └──────────────────────────────────────────────────────────────────┘

add_executable (Markr ${SRC} ${QRC})
qt5_use_modules (Markr Sql Widgets)

# ┌──────────────────────────────────────────────────────────────────┐
# │  Utility (copy dlls to build dir, set release mode)              │
# └──────────────────────────────────────────────────────────────────┘

set (MARKR_RELEASE_OPTIONS "-O3")

# target_compile_options(Markr PUBLIC "$<$<CONFIG:DEBUG>:${MARKR_DEBUG_OPTIONS}>")
target_compile_options(Markr PUBLIC "$<$<CONFIG:RELEASE>:${MARKR_RELEASE_OPTIONS}>")

# target_include_directories(Markr PUBLIC "${PROJECT_SOURCE_DIR}/src/extern")

# https://gist.github.com/Rod-Persky/e6b93e9ee31f9516261b
if (WIN32)
	if (CMAKE_BUILD_TYPE STREQUAL Release)
		set_property (TARGET Markr PROPERTY WIN32_EXECUTABLE true)
	endif()

	get_target_property (QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
	get_filename_component (QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
	set (QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

	add_custom_command (
		TARGET Markr POST_BUILD
		COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE_DIR:Markr>)
endif (WIN32)