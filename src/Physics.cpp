#include "Physics.h"
#include "Level.h"
#include "Unit.h"

////////////////////////////////////////////////////////////////////////////////
namespace Physics {

////////////////////////////////////////////////////////////////////////////////
bool Collides(const Level &level, const Unit &unit)
{
    const auto unitPos = unit.GetPosition();
    const auto unitSize = unit.GetSize();
    const auto colPix = sf::Vector2f(unitPos.x + unitSize.x / 2, unitPos.y + unitSize.y);
    const auto pix = level.GetPixel(colPix);

    return pix != sf::Color::White;
}

} /* ns Physics */