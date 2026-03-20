set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Plankton main.cpp
        src/Monod.cpp
        inc/Monod.h
        src/ReactorGeometry.cpp
        inc/ReactorGeometry.h
        inc/MonodState.h
        inc/MonodParameters.h
        inc/SimulationParameters.h)

add_executable(PlanktonTests
        test_main.cpp
        src/Monod.cpp
        inc/Monod.h
        src/Monod_tests.cpp
        src/ReactorGeometry.cpp
        inc/ReactorGeometry.h
        src/ReactorGeometry_tests.cpp
        inc/MonodState.h
        inc/MonodParameters.h
        inc/SimulationParameters.h
)

include_directories(inc)

target_link_libraries(PlanktonTests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(PlanktonTests)

