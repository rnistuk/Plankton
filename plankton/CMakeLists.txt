set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Plankton main.cpp
        Monod.cpp
        Monod.h)

add_executable(PlanktonTests
        tests/test_main.cpp
        Monod.cpp
        Monod.h
        Monod_tests.cpp
)

target_link_libraries(PlanktonTests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(PlanktonTests)

