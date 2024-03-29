#pragma once

////////////////////////////////////////////////////////////////////////////////
#include "Vector.h"
#include "Pixel.h"
#include "Projectile.h"
#include <chrono>
#include <unordered_map>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace Physics {

////////////////////////////////////////////////////////////////////////////////
constexpr auto FIXED_PHYSICS_STEP = std::chrono::milliseconds(10);

////////////////////////////////////////////////////////////////////////////////
struct DynamicBody {

    ////////////////////////////////////////////////////////////////////////////
    Vec2f position;

    ////////////////////////////////////////////////////////////////////////////
    Vec2f velocity{0.F, 0.F};

    ////////////////////////////////////////////////////////////////////////////
    float inverse_mass{1.F / 100.F};

    ////////////////////////////////////////////////////////////////////////////
    std::vector<std::uint8_t> pixels;

    ////////////////////////////////////////////////////////////////////////////
    Vec2u size;
};

////////////////////////////////////////////////////////////////////////////////
int CountOverlap(const Pixels& pixels, const DynamicBody& body);

////////////////////////////////////////////////////////////////////////////////
class System {
public:

    ////////////////////////////////////////////////////////////////////////////
    size_t AddDynamicBody(Vec2f position,
                          Vec2f velocity,
                          float inverseMass,
                          std::vector<std::uint8_t> pixels,
                          Vec2u size);

    ////////////////////////////////////////////////////////////////////////////
    const DynamicBody& Get(size_t id);

    ////////////////////////////////////////////////////////////////////////////
    void Update();

    ////////////////////////////////////////////////////////////////////////////
    void SetTerrainPixels(std::pair<std::vector<uint8_t>, Vec2u> terrain);

    ////////////////////////////////////////////////////////////////////////////
    void SetProjectile(Projectile* proj);

private:

    /// Next unused ID
    ////////////////////////////////////////////////////////////////////////////
    size_t nextID = 0;

    ////////////////////////////////////////////////////////////////////////////
    std::unordered_map<size_t, DynamicBody> bodies;

    ////////////////////////////////////////////////////////////////////////////
    float deltaTime = 1.F / 100.F;

    ////////////////////////////////////////////////////////////////////////////
    std::vector<uint8_t> terrainPixels;

    ////////////////////////////////////////////////////////////////////////////
    Vec2u terrainDimensions;

    ////////////////////////////////////////////////////////////////////////////
    Projectile* projectile = nullptr;

};

} /* ns Physics */
