set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLANKTON_SOURCES
        src/Monod.cpp
        src/beerLambert.cpp
        src/CsvExport.cpp
        src/Simulation.cpp)

add_library(plankton_lib STATIC ${PLANKTON_SOURCES})

target_include_directories(plankton_lib PUBLIC inc)

add_executable(PlanktonTests test_main.cpp
        src/Monod_tests.cpp
        src/ReactorGeometry_tests.cpp
        src/CsvExport_tests.cpp
        src/Monod_Mortality_tests.cpp
)

target_link_libraries(PlanktonTests
        PRIVATE
        plankton_lib
        GTest::gtest_main
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(PlanktonTests)
