#pragma once

////////////////////////////////////////////////////////////////////////////////
#include "Level.h"
#include "StateMachine.h"
#include "TextureCache.h"
#include "Players.h"
#include "Projectile.h"
#include "WorldScroll.h"
#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
constexpr unsigned int LEVEL_WIDTH = 4000;
constexpr unsigned int LEVEL_HEIGHT = 1000;
constexpr sf::Int32 FIXED_FRAME_TIME_MILLIS = 10; 
constexpr float SHELL_VELOCITY = 700.F;

////////////////////////////////////////////////////////////////////////////////
class SinglePlayer : public GameState {
public:

    ////////////////////////////////////////////////////////////////////////////
    SinglePlayer(sf::RenderWindow& win, TextureCache& cache); 

    ////////////////////////////////////////////////////////////////////////////
    void ExecuteFrame() final;    

private:

    ////////////////////////////////////////////////////////////////////////////
    void doPhysics(const sf::Time& deltaTime);

    ////////////////////////////////////////////////////////////////////////////
    Players players;

    ////////////////////////////////////////////////////////////////////////////
    std::vector<Projectile> shells;

    ////////////////////////////////////////////////////////////////////////////
    sf::Event event{};
    
    ////////////////////////////////////////////////////////////////////////////
    sf::Time frameTimeAccumulator{};
    
    ////////////////////////////////////////////////////////////////////////////
    sf::Clock frameClock{};

    ////////////////////////////////////////////////////////////////////////////
    const sf::Time fixedFrameTime{sf::milliseconds(FIXED_FRAME_TIME_MILLIS)};

    ////////////////////////////////////////////////////////////////////////////
    sf::RenderWindow& window;
    
    ////////////////////////////////////////////////////////////////////////////
    TextureCache& textureCache;

    ////////////////////////////////////////////////////////////////////////////
    Level level{LEVEL_WIDTH, LEVEL_HEIGHT};

    ////////////////////////////////////////////////////////////////////////////
    WorldScroll scroll;
};

