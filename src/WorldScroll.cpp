#include "WorldScroll.h"

////////////////////////////////////////////////////////////////////////////////
WorldScroll::WorldScroll(sf::Vector2u wSize, sf::Vector2u sSize)
    : offset(sSize / 2U), worldSize(wSize), screenSize(sSize) {
    view.setCenter(static_cast<float>(offset.x), static_cast<float>(offset.y));
    view.setSize(static_cast<float>(screenSize.x), static_cast<float>(screenSize.y));
}

////////////////////////////////////////////////////////////////////////////////
void WorldScroll::SetScreenSize(unsigned long width, unsigned long height) {
    screenSize = sf::Vector2u(width, height);
    view.setSize(static_cast<float>(screenSize.x), static_cast<float>(screenSize.y));
}

////////////////////////////////////////////////////////////////////////////////
sf::View WorldScroll::GetView() const {
    return view;
}

////////////////////////////////////////////////////////////////////////////////
void WorldScroll::Scroll(sf::Vector2f direction, sf::Time deltaTime) {
    offset.x += static_cast<unsigned long>(direction.x * deltaTime.asSeconds() * ScrollSpeed);
    offset.y += static_cast<unsigned long>(direction.y * deltaTime.asSeconds() * ScrollSpeed);

    const auto min = screenSize / 2U;
    const auto max = worldSize - min;

    if( screenSize.y > worldSize.y ) {
        const auto yExtraHalf = (screenSize.y - worldSize.y) / 2U;
        offset.y = min.y - yExtraHalf;
    } else {
        offset.y = std::clamp(offset.y, min.y, max.y);
    }

    if( screenSize.x > worldSize.x ) {
        const auto xExtraHalf = (screenSize.x - worldSize.x) / 2U;
        offset.x = min.x - xExtraHalf;
    } else {
        offset.x = std::clamp(offset.x, min.x, max.x);
    }

    view.setCenter(static_cast<float>(offset.x), static_cast<float>(offset.y));
}