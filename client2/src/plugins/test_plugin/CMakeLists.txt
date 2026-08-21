cmake_minimum_required(VERSION 3.19)
project(test_plugin VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilita a geração automática de MOC e RCC pelo Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Define o plugin como um módulo compartilhado (.so / .dll)
add_library(test_plugin MODULE
    src/TestButtonPlugin.h
    src/TestButtonPlugin.cpp
)

# Adiciona o diretório da interface IPlugin.h (ajuste o caminho relativo conforme sua estrutura)
target_include_directories(test_plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/IPlugin.h
)

# Vincula o plugin aos módulos do Qt
target_link_libraries(test_plugin PRIVATE
    Qt::Core
    Qt::Gui
)

# Define a pasta de saída do .so/dll para o diretório de plugins da aplicação
set_target_properties(test_plugin PROPERTIES
    PREFIX "" # Remove o prefixo 'lib' padrão do Linux (gera test_plugin.so em vez de libtest_plugin.so)
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
)