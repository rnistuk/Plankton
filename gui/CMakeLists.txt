set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Charts)
include_directories(src)
add_executable(gui main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/InitialConditionsPanel.cpp
        src/InitialConditionsPanel.h
        src/KineticParametersPanel.cpp
        src/KineticParametersPanel.h
        src/ReactorGeometryPanel.cpp
        src/ReactorGeometryPanel.h
        src/Results.cpp
        src/Results.h
        src/SimulationControlsPanel.cpp
        src/SimulationControlsPanel.h
        src/SimulationRunner.cpp
        src/SimulationRunner.h
        src/ParameterPanel.cpp
        src/ParameterPanel.h)
target_link_libraries(gui PRIVATE Qt6::Core Qt6::Widgets Qt6::Charts plankton_lib)

add_executable(GuiTests test_main.cpp
        src/InitialConditionsPanel.cpp
        src/InitialConditionsPanel.h
        src/InitialConditionsPanel_tests.cpp
        src/ParameterPanel.cpp
        src/ParameterPanel.h
        src/SimulationControlsPanel.h
        src/SimulationControlsPanel.cpp
        src/SimulationRunner.cpp
        src/SimulationRunner.h
        src/ReactorGeometryPanel.cpp
        src/ReactorGeometryPanel.h
        src/ReactorGeometryPanel_tests.cpp
        src/KineticParametersPanel.cpp
        src/KineticParametersPanel.h
        src/KineticParametersPanel_tests.cpp
        src/SimulationControlsPanel_tests.cpp
        src/Results.cpp
        src/Results.h
        src/Results_tests.cpp
)

target_link_libraries(GuiTests
        PRIVATE
        Qt6::Charts
        Qt6::Core
        Qt6::Widgets
        plankton_lib
        GTest::gtest
        GTest::gmock)

include(GoogleTest)
gtest_discover_tests(GuiTests)
