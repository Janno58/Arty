#pragma once

////////////////////////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>

////////////////////////////////////////////////////////////////////////////////
constexpr float ScrollSpeed = 200.F;

////////////////////////////////////////////////////////////////////////////////
class WorldScroll {
public:

    ////////////////////////////////////////////////////////////////////////////
    WorldScroll(sf::Vector2u worldSize, sf::Vector2u screenSize);

    ////////////////////////////////////////////////////////////////////////////
    void SetScreenSize(unsigned long width, unsigned long height);

    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    sf::View GetView() const;

    ////////////////////////////////////////////////////////////////////////////
    void Scroll(sf::Vector2f direction, sf::Time deltaTime);

private:

    ////////////////////////////////////////////////////////////////////////////
    sf::View view;

    ////////////////////////////////////////////////////////////////////////////
    sf::Vector2u offset;

    ////////////////////////////////////////////////////////////////////////////
    sf::Vector2u worldSize;

    ////////////////////////////////////////////////////////////////////////////
    sf::Vector2u screenSize;

};
