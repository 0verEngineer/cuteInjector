cmake_minimum_required(VERSION 3.21)
project(cuteInjector VERSION 0.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp

    src/helpers/jsonSerializer.h

    src/ui/mainWindow.cpp
    src/ui/mainWindow.h
    src/ui/mainWindow.ui
    src/ui/selectProcessDialog.cpp
    src/ui/selectProcessDialog.h
    src/ui/style.h
    src/ui/textInfoWindow.cpp
    src/ui/textInfoWindow.h

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


qt_add_executable(cuteInjector
    MANUAL_FINALIZATION
    ${SOURCES}
    ${RC_FILE}
)

# Autogen files in visual studio
target_include_directories(cuteInjector
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/cuteInjector_autogen/include
)

target_link_libraries(cuteInjector PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(cuteInjector PROPERTIES
    WIN32_EXECUTABLE TRUE
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS /MACHINE:X64"
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cuteInjector)
endif()
