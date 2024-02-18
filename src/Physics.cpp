#include "Physics.h"
#include "Projectile.h"
#include "Vector.h"
#include "Config.h"
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
namespace Physics {

////////////////////////////////////////////////////////////////////////////////
System::System() {
    lastUpdate = std::chrono::steady_clock::now();
}

////////////////////////////////////////////////////////////////////////////////
size_t System::AddDynamicBody(Vec2f position,
                              Vec2f velocity,
                              float inverseMass,
                              std::vector<std::uint8_t> pixels,
                              Vec2u size) {
    bodies.emplace(nextID, DynamicBody(position, velocity, inverseMass, std::move(pixels), size));
    return nextID++;
}

////////////////////////////////////////////////////////////////////////////////
const DynamicBody& System::Get(size_t id) {
    if(!bodies.contains(id)) {
        throw std::logic_error("No such ID in bodies!");
    }

    return bodies[id];
}

////////////////////////////////////////////////////////////////////////////////
int CountOverlap(const Pixels &pixels, const DynamicBody &body) {
    int count = 0;

    const auto minX = static_cast<int>(body.position.x);
    const auto minY = static_cast<int>(body.position.y);
    const auto maxX = static_cast<int>(body.size.x);
    const auto maxY = static_cast<int>(body.size.y);

    for (const auto& pix : pixels) {
        if (pix.x > minX && pix.y > minY && pix.x < minX + maxX && pix.y < minY + maxY) {
            const auto x_pos = pix.x - minX;
            const auto y_pos = pix.y - minY;
            auto index = (x_pos + y_pos * maxX) * 4;

            assert(index < body.pixels.size());

            for (auto offset = 0UL; offset < 4; offset++) {
                if (body.pixels[index + offset] > 0) {
                    count++;
                    break;
                }
            }
        }
    }

    return count;
}

////////////////////////////////////////////////////////////////////////////////
void System::Update() {
    auto now = std::chrono::steady_clock::now();
    accumulator += (now - lastUpdate);
    lastUpdate = std::chrono::steady_clock::now();

    while (accumulator >= FIXED_PHYSICS_STEP) {
        accumulator -= FIXED_PHYSICS_STEP;

        for (auto& body : bodies) {

            body.second.velocity += (GRAVITY / body.second.inverse_mass) * deltaTime;
            body.second.position += body.second.velocity * deltaTime;

            if(Overlaps(body.second.position, body.second.size, body.second.pixels,
                        Vec2f(0.F, 0.F), terrainDimensions, terrainPixels)) {
                body.second.velocity = -GRAVITY;
            }
        }

        if (projectile != nullptr && !projectile->has_collided) {
            projectile->velocity += (GRAVITY / projectile->inverse_mass) * deltaTime;
            projectile->position += projectile->velocity * deltaTime;
            projectile->rotation = std::atan2(projectile->velocity.y, projectile->velocity.x) * 180.F / M_PIf;

            auto tip = RotateAroundPoint(Vec2f(0.F, 0.F), Vec2f(18.F, 6.F), projectile->rotation);
            auto ground_collision = Overlaps(tip + projectile->position, Vec2f(0.F, 0.F), terrainDimensions, terrainPixels);
            bool tank_collision = false;

            for (auto& body : bodies) {
                if(Overlaps(tip + projectile->position, body.second.position, body.second.size, body.second.pixels)) {
                    tank_collision = true;
                    break;
                }
            }

            projectile->has_collided = ground_collision || tank_collision;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void System::SetTerrainPixels(std::pair<std::vector<uint8_t>, Vec2u> terrain) {
    terrainPixels = std::move(terrain.first);
    terrainDimensions = terrain.second;

}

////////////////////////////////////////////////////////////////////////////////
void System::SetProjectile(Projectile *proj) {
    projectile = proj;
}

} /* ns Physics */
