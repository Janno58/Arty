#include "Projectile.h"
#include "Config.h"

////////////////////////////////////////////////////////////////////////////////
constexpr float TURRET_SCALE = 0.25F;

////////////////////////////////////////////////////////////////////////////////
TextureCache* Projectile::texCache = nullptr;

////////////////////////////////////////////////////////////////////////////////
Projectile::Projectile(sf::Vector2f pos, sf::Vector2f vel) : velocity(vel) {
    if(texCache == nullptr) {
        throw std::logic_error("Projectile::Projectile:: texCache not set!");
    }

    auto& tex = texCache->GetTexture("tank_bullet3.png");

    sprite.setTexture(tex);
    sprite.setPosition(pos);
    sprite.scale(TURRET_SCALE, TURRET_SCALE);

    auto angle = std::atan2(vel.y, vel.x) * 180.F / M_PI;
    sprite.setRotation(angle);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<Pixel> Projectile::Explode() {
    exploded = true;
    auto pos = GetTheTip();
    int xPos = static_cast<int>(pos.x);
    int yPos = static_cast<int>(pos.y);

    return CreateCircle(xPos, yPos, 50);
}

////////////////////////////////////////////////////////////////////////////////
bool Projectile::HasExploded() const {
    return exploded;
}

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f Projectile::GetTheTip() const  {
    auto xOff = sprite.getLocalBounds().width;
    auto yOff = sprite.getLocalBounds().height / 2;

    return sprite.getTransform().transformPoint(xOff, yOff);
}

////////////////////////////////////////////////////////////////////////////////
void Projectile::StepPhysics(float deltaTime) {
    if(exploded) {
        return;
    }

    velocity += (GRAVITY / inverseMass) * deltaTime;
    auto pos = sprite.getPosition();
    sprite.setPosition(pos + (velocity * deltaTime));

    auto dir = velocity;
    auto angle = std::atan2(dir.y, dir.x) * 180.F / M_PI;
    sprite.setRotation(angle);
}

////////////////////////////////////////////////////////////////////////////////
void Projectile::SetTexCache(TextureCache *cache) {
    texCache = cache;
}

////////////////////////////////////////////////////////////////////////////////
void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if(exploded) {
        return;
    }

    target.draw(sprite, states);
}

////////////////////////////////////////////////////////////////////////////////
ShellOutsideLevel::ShellOutsideLevel(unsigned int width, unsigned int height)
: Width(static_cast<float>(width)), Height(static_cast<float>(height)) {

}

////////////////////////////////////////////////////////////////////////////////
bool ShellOutsideLevel::operator()(const Projectile &shell) const {
    const auto pos = shell.GetTheTip();
    return pos.x > Width || pos.x < 0 || pos.y > Height;

}

