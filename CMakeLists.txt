# Definir a versão mínima do CMake necessária
cmake_minimum_required(VERSION 3.16)

# Definir o nome do projeto
project(dfe_app VERSION 0.0.1)

# Ativar padrões do C++
set(CMAKE_CXX_STANDARD 17)

# find_package(PkgConfig REQUIRED)

# adicionar Qt Widgets além de Sql e ativar automoc
find_package(Vulkan REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Sql Widgets Core Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# add_library(plugin_api INTERFACE)
# target_include_directories(plugin_api INTERFACE ${CMAKE_SOURCE_DIR}/include)

file(COPY src/migrations DESTINATION ./)
file(COPY src/config.json DESTINATION ./)

add_executable(dfe_app
        src/main.cpp
        src/database/database.cpp
        src/forms/MainWindow.cpp)

target_link_libraries(dfe_app PRIVATE Qt6::Sql Qt6::Widgets Qt6::Core Qt6::Gui)
# target_link_libraries(dfe_app PRIVATE plugin_api dl Qt6::Widgets Qt6::Sql Qt6::Core)