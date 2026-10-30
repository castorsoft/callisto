cmake_minimum_required(VERSION 3.19)
project(callisto LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Gui Concurrent Network)
# add_subdirectory(../../zafra zafra) # If we want to build it together
# Actually, the user says "Os fontes da lib estão em ../../zafra". Let's add it.

add_subdirectory(../zafra zafra)

set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

# 1. Agrupar os arquivos de negocio em uma biblioteca estatica
file(GLOB_RECURSE CORE_LIB_SOURCES
    "src/application/*.cpp"
    "src/application/*.h"
    "src/core/*.cpp"
    "src/core/*.h"
    "src/infrastructure/*.cpp"
    "src/infrastructure/*.h"
    "src/plugins/*.cpp"
    "src/plugins/*.h"
)

qt_add_library(core_lib STATIC ${CORE_LIB_SOURCES})

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(core_lib PUBLIC
    Qt::Core
    Qt::Sql
    Qt::Network
#    Qt::Widgets
#    Qt::Gui
    Qt::Concurrent
    zafra
)


# 2. Executavel principal da aplicacao
file(GLOB_RECURSE APP_SOURCES
    "src/presentation/*.cpp"
    "src/presentation/*.h"
    "src/presentation/*.ui"
    "src/presentation/resources/*.qrc"
    "src/presentation/plugins/*.cpp"
    "src/presentation/plugins/*.h"
    "src/main.cpp"
)

qt_add_executable(callisto ${APP_SOURCES}
    # src/assets/images.qrc
)

# callisto consome a core_lib
target_link_libraries(callisto PRIVATE core_lib Qt::Widgets Qt::Gui)
set_target_properties(callisto PROPERTIES ENABLE_EXPORTS ON)

option(HEADLESS "Build as a terminal application without GUI" OFF)
if(HEADLESS)
    target_compile_definitions(callisto PRIVATE HEADLESS_MODE)
endif()

# 3. Habilitar testes
enable_testing()
add_subdirectory(tests)

# install(TARGETS callisto
#     BUNDLE  DESTINATION .
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

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

# add_subdirectory(external_plugins/test_plugin)
add_subdirectory(external_plugins/sped_fiscal)
add_subdirectory(qt_plugins)

# qt_generate_deploy_app_script(
#     TARGET callisto
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )
