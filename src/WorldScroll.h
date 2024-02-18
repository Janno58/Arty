#pragma once

////////////////////////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>

////////////////////////////////////////////////////////////////////////////////
constexpr float ScrollSpeed = 200.F;
constexpr float MaxFocusHeight = -2000.F;

////////////////////////////////////////////////////////////////////////////////
class WorldScroll {
public:

    ////////////////////////////////////////////////////////////////////////////
    WorldScroll(sf::Vector2f worldSize, sf::Vector2u screenSize);

    ////////////////////////////////////////////////////////////////////////////
    void SetScreenSize(float width, float height);

    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    sf::View GetView() const;

    ////////////////////////////////////////////////////////////////////////////
    void Focus(sf::Vector2f center);
    
    ////////////////////////////////////////////////////////////////////////////
    void Scroll(sf::Vector2f direction, float delta_time);

private:

    ////////////////////////////////////////////////////////////////////////////
    sf::View view;

    ////////////////////////////////////////////////////////////////////////////
    sf::Vector2f offset;

    ////////////////////////////////////////////////////////////////////////////
    sf::Vector2f worldSize;

    ////////////////////////////////////////////////////////////////////////////
    sf::Vector2f screenSize;

};
