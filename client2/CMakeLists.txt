cmake_minimum_required(VERSION 3.19)
project(client2 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)
find_package(KF6WidgetsAddons REQUIRED)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

qt_add_executable(client2
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/forms/mainwindow.cpp
    src/forms/mainwindow.h
    src/forms/mainwindow.ui
    src/assets/images.qrc
    src/data/migrationrunner.cpp
    src/data/migrationrunner.h
    src/data/databasemanager.h
    src/data/databasemanager.cpp
    src/data/configmanager.h
    src/data/configmanager.cpp
)

target_link_libraries(client2
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        KF6::WidgetsAddons
)

install(TARGETS client2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config.json)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config.json
        ${CMAKE_CURRENT_BINARY_DIR}/config.json
        COPYONLY
    )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/migrations)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/migrations DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

qt_generate_deploy_app_script(
    TARGET client2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
