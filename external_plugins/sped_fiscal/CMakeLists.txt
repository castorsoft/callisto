cmake_minimum_required(VERSION 3.19)
project(sped_fiscal VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE SPED_PLUGIN_SOURCES
    "src/*.h"
    "src/*.cpp"
)

add_library(sped_fiscal MODULE
    ${SPED_PLUGIN_SOURCES}
)

target_include_directories(sped_fiscal PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(sped_fiscal PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Sql
)

set_target_properties(sped_fiscal PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
)
