cmake_minimum_required(VERSION 3.21)
project(cuteInjector VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(x86Bridge main.cpp)

set_target_properties(x86Bridge PROPERTIES
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /MACHINE:X86"
)
