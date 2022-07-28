#include "Input.h"

////////////////////////////////////////////////////////////////////////////////
namespace Input {

////////////////////////////////////////////////////////////////////////////////
auto GetMapScroll() -> sf::Vector2f
{
    sf::Vector2f vec;

    if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A) ) {
        vec += {-1.F, 0.F};
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D) ) {
        vec += {1.F, 0.F};
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
       sf::Keyboard::isKeyPressed(sf::Keyboard::W) ) {
        vec += {0.F, -1.F};
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S) ) {
        vec += {0.F, 1.F};
    }

    return vec;
}

} /* ns Input */