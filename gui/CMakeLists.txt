set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Charts)
include_directories(src)
add_executable(gui main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/InitialConditions.cpp
        src/InitialConditions.h
        src/KineticParameters.cpp
        src/KineticParameters.h
        src/ReactorGeometry.cpp
        src/ReactorGeometry.h
        src/Results.cpp
        src/Results.h
        src/SimulationRunner.cpp
        src/SimulationRunner.h
        src/ParameterPanel.cpp
        src/ParameterPanel.h)
target_link_libraries(gui PRIVATE Qt6::Core Qt6::Widgets Qt6::Charts plankton_lib)
