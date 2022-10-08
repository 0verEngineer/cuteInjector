cmake_minimum_required(VERSION 3.21)
project(CuteInjector VERSION 0.1.0 LANGUAGES CXX)

set(CUTE_INJECTOR_VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/Settings.cpp src/Settings.h

    src/data/jsonSerializer.cpp
    src/data/jsonSerializer.h

    src/ui/mainWindow.cpp
    src/ui/mainWindow.h
    src/ui/mainWindowActions.cpp
    src/ui/mainWindow.ui
    src/ui/style.h
    src/ui/textInfoWindow.cpp
    src/ui/textInfoWindow.h
    src/ui/menuBar.h src/ui/menuBar.cpp

    src/ui/dialogs/selectProcessDialog.cpp
    src/ui/dialogs/selectProcessDialog.h
    src/ui/dialogs/settings/SettingsDialog.cpp src/ui/dialogs/settings/SettingsDialog.h
    src/ui/dialogs/settings/HotKeySettingsTab.cpp src/ui/dialogs/settings/HotKeySettingsTab.h
    src/ui/dialogs/settings/HotKeySettingsLine.cpp src/ui/dialogs/settings/HotKeySettingsLine.h

    src/utils/dllFile.cpp
    src/utils/dllFile.h
    src/utils/injector/injector.cpp
    src/utils/injector/injector.h
    src/utils/process/process.cpp
    src/utils/process/process.h
    src/utils/process/processManager.cpp
    src/utils/process/processManager.h
)


# Icon
enable_language("RC")
set(RC_FILE resources/cuteInjector.rc)


qt_add_executable(CuteInjector
    MANUAL_FINALIZATION
    ${SOURCES}
    ${RC_FILE}
)

# Autogen files in visual studio
target_include_directories(CuteInjector
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/cuteInjector_autogen/include
)

target_link_libraries(CuteInjector PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(CuteInjector PROPERTIES
    WIN32_EXECUTABLE TRUE
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS /MACHINE:X64"
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CuteInjector)
endif()
