////////////////////////////////////////////////////////////////////////////////
#include "Players.h"
#include "Unit.h"
#include "Vector.h"
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
constexpr float RAD_TO_DEG = 180.F / M_PI;

////////////////////////////////////////////////////////////////////////////////
const Unit& Player::GetDrawable() const {
    return drawable;
}

////////////////////////////////////////////////////////////////////////////////
Unit& Player::GetDrawable() {
    return drawable;
}

////////////////////////////////////////////////////////////////////////////////
std::pair<sf::Vector2f, sf::Vector2f> Player::GetShellPos() const {
    const auto unit = GetDrawable();
    const auto posBase = unit.GetRotationPoint();
    const auto posTip = unit.GetMuzzlePos();
    const auto direction = Normalize(posTip - posBase);

    return std::make_pair(posTip, direction);
}

////////////////////////////////////////////////////////////////////////////////
void HumanPlayer::MouseMove(const sf::Vector2f& pos) {
    drawable.MouseMove(pos);
}

////////////////////////////////////////////////////////////////////////////////
Player* Players::GetActive() {
    assert(activeIndex < players.size());

    return players[activeIndex].get();
}

////////////////////////////////////////////////////////////////////////////////
bool Players::PlayerChanged() {
    if(activePlayerChanged) {
        activePlayerChanged = false;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Players::Next() {
    activePlayerChanged = true;
    activeIndex++;

    if(activeIndex >= players.size()) {
        activeIndex = 0UL;
    }
}
