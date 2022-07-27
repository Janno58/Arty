add_executable(arty Arty.cpp
        LevelGen.cpp LevelGen.h
        Level.cpp Level.h
        WorldScroll.cpp WorldScroll.h)

set_target_properties(arty PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(arty sfml-window sfml-graphics)

