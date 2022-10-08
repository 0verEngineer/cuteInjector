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
    src/Main.cpp
    src/Settings.cpp src/Settings.h

    src/data/JsonSerializer.cpp
    src/data/JsonSerializer.h

    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/MainWindowActions.cpp
    src/ui/MainWindow.ui
    src/ui/Style.h
    src/ui/TextInfoWindow.cpp
    src/ui/TextInfoWindow.h
    src/ui/MenuBar.h src/ui/MenuBar.cpp

    src/ui/dialogs/SelectProcessDialog.cpp
    src/ui/dialogs/SelectProcessDialog.h
    src/ui/dialogs/settings/SettingsDialog.cpp src/ui/dialogs/settings/SettingsDialog.h
    src/ui/dialogs/settings/HotKeySettingsTab.cpp src/ui/dialogs/settings/HotKeySettingsTab.h
    src/ui/dialogs/settings/HotKeySettingsLine.cpp src/ui/dialogs/settings/HotKeySettingsLine.h

    src/utils/DllFile.cpp
    src/utils/DllFile.h
    src/utils/injector/Injector.cpp
    src/utils/injector/Injector.h
    src/utils/process/Process.cpp
    src/utils/process/Process.h
    src/utils/process/ProcessManager.cpp
    src/utils/process/ProcessManager.h
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
