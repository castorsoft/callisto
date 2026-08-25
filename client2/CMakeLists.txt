cmake_minimum_required(VERSION 3.19)
project(client2 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Gui)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

# 1. Agrupar os arquivos de negocio em uma biblioteca estatica
qt_add_library(core_lib STATIC
    src/infrastructure/database/MigrationRunner.cpp
    src/infrastructure/database/MigrationRunner.h
    src/infrastructure/database/DatabaseManager.h
    src/infrastructure/database/DatabaseManager.cpp
    src/infrastructure/config/ConfigManager.h
    src/infrastructure/config/ConfigManager.cpp
    src/infrastructure/plugins/IPlugin.h
    src/infrastructure/plugins/PluginManager.h
    src/infrastructure/plugins/PluginManager.cpp
    src/application/services/ServiceManager.h
    src/application/services/ServiceManager.cpp
    src/application/services/IService.h
    src/application/workers/DirectoryWatcherWorker.h
    src/application/workers/DirectoryWatcherWorker.cpp
)

target_link_libraries(core_lib PRIVATE Qt::Core Qt::Sql Qt::Widgets Qt::Gui)
target_include_directories(core_lib PUBLIC src)

# 2. Executavel principal da aplicacao
qt_add_executable(client2
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/presentation/forms/MainWindow.cpp
    src/presentation/forms/MainWindow.h
    src/presentation/forms/MainWindow.ui
    src/assets/images.qrc
)

# client2 consome a core_lib
target_link_libraries(client2 PRIVATE core_lib Qt::Core Qt::Widgets Qt::Sql Qt::Gui)

# 3. Habilitar testes
enable_testing()
add_subdirectory(tests)

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

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/files)

add_subdirectory(src/plugins/test_plugin)

qt_generate_deploy_app_script(
    TARGET client2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
