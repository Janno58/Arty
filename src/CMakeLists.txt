add_executable(arty Arty.cpp
        Level.cpp Level.h
        WorldScroll.cpp WorldScroll.h
        Physics.cpp Physics.h
        platform/SFML_Input.cpp platform/SFML_Input.h
        Config.h
        Vector.h
        Projectile.cpp Projectile.h
        TextureCache.cpp TextureCache.h
        Pixel.h Pixel.cpp
        Background.cpp Background.h
        Players.cpp Players.h
        StateMachine.cpp StateMachine.h
        Game.cpp Game.h
        CompPlayer.cpp CompPlayer.h
        TextKeeper.cpp TextKeeper.h
        gui/GuiFunctions.cpp gui/GuiFunctions.h
        gui/Button.cpp gui/Button.h
        gui/TextEdit.cpp
        gui/TextEdit.h
        Player.cpp
        Player.h
        gui/Config.h
        platform/SFML_Drawer.cpp platform/SFML_Drawer.hpp
        Types.h)

set_target_properties(arty PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(arty sfml-window sfml-graphics)

