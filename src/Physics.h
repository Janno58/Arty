#pragma once

////////////////////////////////////////////////////////////////////////////////
class Level;
class Unit;
class Projectile;

////////////////////////////////////////////////////////////////////////////////
namespace Physics {

////////////////////////////////////////////////////////////////////////////////
bool Collides(const Level& level, const Unit& unit);

////////////////////////////////////////////////////////////////////////////////
bool Collides(const Level& level, const Projectile& shell);

} /* ns Physics */
