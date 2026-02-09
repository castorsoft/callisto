# Definir a versão mínima do CMake necessária
cmake_minimum_required(VERSION 3.16)

# Definir o nome do projeto
project(dfe_app VERSION 0.0.1)

# Ativar padrões do C++
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)

# adicionar Qt Widgets além de Sql e ativar automoc
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
set(CMAKE_AUTOMOC ON)

add_library(plugin_api INTERFACE)
target_include_directories(plugin_api INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(dfe_app src/main.cpp src/initworker.cpp)

target_link_libraries(dfe_app PRIVATE plugin_api dl Qt6::Widgets Qt6::Sql Qt6::Core)
