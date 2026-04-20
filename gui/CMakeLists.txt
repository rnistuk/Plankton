
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Charts)
include_directories(src)
qt_add_executable(gui main.cpp
        src/PlanktonWizard.cpp
        src/PlanktonWizard.h
        src/PlanktonWizardPage.h
        src/InitialConditionsPage.cpp
        src/InitialConditionsPage.h
        src/KineticParametersPage.cpp
        src/KineticParametersPage.h
        src/ReactorGeometryPage.cpp
        src/ReactorGeometryPage.h
        src/ResultsPage.cpp
        src/ResultsPage.h
        src/ConfigAssembler.h
        src/ConfigAssembler.cpp)
target_link_libraries(gui PRIVATE Qt6::Widgets Qt6::Charts plankton_lib)




