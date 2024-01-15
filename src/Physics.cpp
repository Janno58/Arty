#include "Physics.h"
#include "Projectile.h"
#include "Level.h"
#include "Unit.h"

////////////////////////////////////////////////////////////////////////////////
namespace Physics {

////////////////////////////////////////////////////////////////////////////////
bool Collides(const Level& level, const Unit& unit)
{
    const auto unitPos = unit.GetPosition();
    const auto unitSize = unit.GetSize();
    const auto colPix = sf::Vector2f(unitPos.x + unitSize.x / 2, unitPos.y + unitSize.y);
    const auto pix = level.GetPixel(colPix);

    return pix != sf::Color(0,0,0,0);
}

////////////////////////////////////////////////////////////////////////////////
bool Collides(const Level& level, const Projectile& shell)
{
    const auto tip = shell.GetTheTip();
    const auto pix = level.GetPixel(tip);

    return pix != sf::Color(0,0,0,0);
}

////////////////////////////////////////////////////////////////////////////////
bool Collides(const Unit& tank, const Projectile& shell) {
    const auto tankBox = tank.GetGlobalBounds();
    const auto tipPos = shell.GetTheTip();

    if( !tankBox.contains(tipPos) ) {
        return false;
    }

    const auto pix = tank.GetPixelGlobal(tipPos);
    auto hit = pix != sf::Color(0,0,0,0);

    return hit;
}

} /* ns Physics */
