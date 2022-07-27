#include "WorldScroll.h"

////////////////////////////////////////////////////////////////////////////////
WorldScroll::WorldScroll(sf::Vector2f wSize, sf::Vector2f sSize)
    : worldSize(wSize), screenSize(sSize) {
    const auto halfWidth = screenSize.x / 2.F;
    const auto halfHeight = screenSize.y / 2.F;

    offset = sf::Vector2f(halfWidth, halfHeight);

    view.setCenter(offset);
    view.setSize(screenSize);
}

////////////////////////////////////////////////////////////////////////////////
void WorldScroll::SetScreenSize(float width, float height) {
    screenSize = sf::Vector2f(width, height);
    view.setSize(width, height);
}

////////////////////////////////////////////////////////////////////////////////
sf::View WorldScroll::GetView() const {
    return view;
}

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f WorldScroll::GetOffset() const {
    return offset;
}

////////////////////////////////////////////////////////////////////////////////
void WorldScroll::Scroll(sf::Vector2f direction, sf::Time deltaTime) {
    const auto magnitude = deltaTime.asSeconds() * ScrollSpeed;
    offset += direction * magnitude;

    const auto min = screenSize / 2.F;
    const auto max = worldSize - min;

    if( screenSize.y > worldSize.y ) {
        const auto yExtraHalf = (screenSize.y - worldSize.y) / 2.F;
        offset.y = min.y - yExtraHalf;
    } else {
        offset.y = std::clamp(offset.y, min.y, max.y);
    }

    if( screenSize.x > worldSize.x ) {
        const auto xExtraHalf = (screenSize.x - worldSize.x) / 2.F;
        offset.x = min.x - xExtraHalf;
    } else {
        offset.x = std::clamp(offset.x, min.x, max.x);
    }

    view.setCenter(offset);
}