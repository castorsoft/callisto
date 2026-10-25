find_package(Qt6 REQUIRED COMPONENTS Test)
# Baixar o Google Test via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
# Make GoogleTest available
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Criar o executavel de testes globais
qt_add_executable(callisto_tests
    # Arquivo de teste de exemplo para validar a configuracao
    domain/DummyTest.cpp
)

# Linkar os testes com o GTest, GMock e com a NOSSA logica (core_lib)
target_link_libraries(callisto_tests
    PRIVATE
    core_lib
    GTest::gtest_main
    GTest::gmock
    Qt::Core
    Qt::Test
)

# Adicionar ao painel de testes do CTest/CMake
include(GoogleTest)
gtest_discover_tests(callisto_tests)
