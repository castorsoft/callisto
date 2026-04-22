# Definir a versão mínima do CMake necessária
cmake_minimum_required(VERSION 3.16)

# Definir o nome do projeto
project(dfe_server VERSION 0.0.1)

# Ativar padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(SOURCES_FILES)

find_package(Drogon REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)

# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)

# add_library(plugin_api INTERFACE)
# target_include_directories(plugin_api INTERFACE ${CMAKE_SOURCE_DIR}/include)

file(COPY src/migrations DESTINATION ./)
file(COPY src/config.json DESTINATION ./)
file(COPY src/server_config.json DESTINATION ./)
make_directory(logs)

aux_source_directory(src/controllers CTL_SRC)
aux_source_directory(src/models MODEL_SRC)
aux_source_directory(src/services SERVICES_SRC)
aux_source_directory(src/repositories REPOSITORIES_SRC)
aux_source_directory(src/dto DTO_SRC)


add_executable(dfe_server)
target_sources(dfe_server
        PRIVATE
        src/main.cpp
        ${DTO_DIR}
        ${SRC_DIR}
        ${CTL_SRC}
        ${REPOSITORIES_SRC}
        ${SERVICES_SRC}
        ${MODEL_SRC})

target_link_libraries(dfe_server PRIVATE Drogon::Drogon Qt6::Core)