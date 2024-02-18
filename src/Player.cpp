#include "Player.h"

////////////////////////////////////////////////////////////////////////////////
std::pair<Vec2f, Vec2f> GetShellPosVel(const Player& player) {
    const auto origin = player.position + TURRET_OFFSET;
    const auto pos = RotateAroundPoint({0.F, 0.F}, MUZZLE_OFFSET, player.turret_angle - TURRET_ANGLE_OFFSET);
    const auto vel = Normalize(pos);

    return {origin, vel};
}
