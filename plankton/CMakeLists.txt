set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLANKTON_SOURCES
        src/Monod.cpp
        inc/Monod.h
        src/ReactorGeometry.cpp
        inc/ReactorGeometry.h
        inc/MonodState.h
        inc/MonodParameters.h
        inc/SimulationParameters.h
        inc/BeerLambert.h
        src/beerLambert.cpp
        src/CsvExport.cpp
        inc/CsvExport.h
        inc/SimulationRecord.h
        inc/Simulation.h)

add_library(plankton_lib STATIC ${PLANKTON_SOURCES})
target_include_directories(plankton_lib PUBLIC inc)

add_executable(PlanktonTests test_main.cpp
        ${PLANKTON_SOURCES}
        src/Monod_tests.cpp
        src/ReactorGeometry_tests.cpp
        src/CsvExport_tests.cpp
        src/Monod_Mortality_tests.cpp
)

include_directories(inc)

target_link_libraries(PlanktonTests
        PRIVATE
        GTest::gtest_main
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(PlanktonTests)

