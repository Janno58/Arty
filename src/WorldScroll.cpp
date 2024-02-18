#include "WorldScroll.h"

////////////////////////////////////////////////////////////////////////////////
WorldScroll::WorldScroll(sf::Vector2f wSize, sf::Vector2u sSize)
    : offset(sSize), worldSize(wSize), screenSize(static_cast<float>(sSize.x), static_cast<float>(sSize.y))  {
    view.setCenter(offset.x, offset.y);
    view.setSize(screenSize.x, screenSize.y);
}

////////////////////////////////////////////////////////////////////////////////
void WorldScroll::SetScreenSize(float width, float height) {
    screenSize = {width, height};
    view.setSize(screenSize.x, screenSize.y);
}

////////////////////////////////////////////////////////////////////////////////
sf::View WorldScroll::GetView() const {
    return view;
}

////////////////////////////////////////////////////////////////////////////////
void WorldScroll::Focus(sf::Vector2f center) {
    offset = center; 
}

////////////////////////////////////////////////////////////////////////////////
void WorldScroll::Scroll(sf::Vector2f direction, float delta_time) {
    offset.x += direction.x * delta_time * ScrollSpeed;
    offset.y += direction.y * delta_time * ScrollSpeed;

    const auto min = screenSize * 0.5F;
    const auto max = worldSize - min;

    offset.y = std::clamp(offset.y, MaxFocusHeight, max.y);

    if( screenSize.x > worldSize.x ) {
        const auto xExtraHalf = (screenSize.x - worldSize.x) / 2.F;
        offset.x = min.x - xExtraHalf;
    } else {
        offset.x = std::clamp(offset.x, min.x, max.x);
    }

    view.setCenter(offset);
}
