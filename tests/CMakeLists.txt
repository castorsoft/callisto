find_package(Qt6 REQUIRED COMPONENTS Core Sql Gui Widgets Test)

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

qt_add_executable(callisto_tests
    core/EntitiesTest.cpp
    core/GlobalStateAndMensagensTest.cpp
    infrastructure/ConfigManagerTest.cpp
    application/ProcessFileTest.cpp
    presentation/ProvidersTest.cpp
    ${CMAKE_SOURCE_DIR}/src/presentation/components/providers/TableDataProviderAdapter.h
    ${CMAKE_SOURCE_DIR}/src/presentation/views/providers/NotaTerceiroListProvider.cpp
    ${CMAKE_SOURCE_DIR}/src/presentation/views/providers/NotaTerceiroItemListProvider.cpp
)

target_include_directories(callisto_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(callisto_tests
    PRIVATE
    core_lib
    GTest::gtest_main
    GTest::gmock
    Qt6::Core
    Qt6::Sql
    Qt6::Gui
    Qt6::Widgets
    Qt6::Test
)

include(GoogleTest)
gtest_discover_tests(callisto_tests)
