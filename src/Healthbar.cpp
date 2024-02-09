#include "Healthbar.h"

////////////////////////////////////////////////////////////////////////////////
Healthbar::Healthbar() {
    bottomBar.setFillColor(sf::Color::Red);
    bottomBar.setSize(size);
    topBar.setFillColor(sf::Color::Green);
    topBar.setSize(size);

    SetHealthPercentage(100.F);
}

////////////////////////////////////////////////////////////////////////////////
void Healthbar::SetHealthPercentage(float healthPercentage) {
    if(healthPercentage < 0.F || healthPercentage > 100.F) {
        throw std::logic_error("Health percentage out of range!");
    }

    const auto newWidth = size.x / 100.F * healthPercentage;
    topBar.setSize({newWidth, size.y});
}

////////////////////////////////////////////////////////////////////////////////
void Healthbar::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(bottomBar, states);
    target.draw(topBar, states);
}

