cmake_minimum_required(VERSION 3.19)
project(client2 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Gui)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

qt_add_executable(client2
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/forms/MainWindow.cpp
    src/forms/MainWindow.h
    src/forms/MainWindow.ui
    src/assets/images.qrc
    src/core/MigrationRunner.cpp
    src/core/MigrationRunner.h
    src/core/DatabaseManager.h
    src/core/DatabaseManager.cpp
    src/core/ConfigManager.h
    src/core/ConfigManager.cpp
    src/core/IPlugin.h
    src/core/PluginManager.h
    src/core/PluginManager.cpp
    src/core/ServiceManager.h
    src/core/IService.h
    src/services/DirectoryWatcherWorker.h
    # src/services/SyncWorker.h
    # src/services/SyncWorker.cpp
)

target_link_libraries(client2
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        Qt::Gui
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

add_subdirectory(src/plugins/test_plugin)

qt_generate_deploy_app_script(
    TARGET client2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
