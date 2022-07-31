#include "Unit.h"
#include "Config.h"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
Unit::Unit(const sf::Texture& tex, const sf::Texture& turretTex, sf::Vector2f spawnPos) {
    sprite.setTexture(tex);
    sprite.scale(scale, scale);

    spriteCenterOffset.x = sprite.getGlobalBounds().width / 2.F;
    spriteCenterOffset.y = sprite.getGlobalBounds().height / 2.F - 20.F;

    turretSprite.setTexture(turretTex);
    turretSprite.scale(scale, scale);
    turretSprite.setOrigin(0.F, turretSprite.getLocalBounds().height / 2);
    turretOffset = sf::Vector2f(sprite.getGlobalBounds().width / 2, 5.F);

    SetPosition(spawnPos);

    pixels = tex.copyToImage();
}

////////////////////////////////////////////////////////////////////////////////
void Unit::MouseMove(sf::Vector2f mousePos) {
    auto unitVec = sprite.getPosition() + spriteCenterOffset - mousePos;

    auto angle = std::atan2(unitVec.y, unitVec.x) * 180.F / M_PI;
    turretSprite.setRotation(angle + 180.F);
}


////////////////////////////////////////////////////////////////////////////////
void Unit::StepPhysics(float dt) {
    velocity += (GRAVITY / inverseMass) * dt;
    auto pos = sprite.getPosition();
    SetPosition(pos + (velocity * dt));
}

////////////////////////////////////////////////////////////////////////////////
sf::FloatRect Unit::GetGlobalBounds() const {
    return sprite.getGlobalBounds();
}

////////////////////////////////////////////////////////////////////////////////
sf::Color Unit::GetPixelGlobal(sf::Vector2f position) const {
    const auto& transform = sprite.getInverseTransform();
    const auto pos = transform.transformPoint(position);
    return pixels.getPixel(static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y));
}

////////////////////////////////////////////////////////////////////////////////
auto Unit::GetPosition() const -> sf::Vector2f {
    return sprite.getPosition();
}

////////////////////////////////////////////////////////////////////////////////
void Unit::SetPosition(sf::Vector2f newPos) {
    sprite.setPosition(newPos);
    turretSprite.setPosition(newPos + turretOffset);
}

////////////////////////////////////////////////////////////////////////////////
auto Unit::GetSize() const -> sf::Vector2f {
    auto width = static_cast<float>(sprite.getLocalBounds().width);
    auto height = static_cast<float>(sprite.getLocalBounds().height);

    return sf::Vector2f(width, height) * scale;
}

////////////////////////////////////////////////////////////////////////////////
auto Unit::GetMuzzlePos() const -> sf::Vector2f {
    auto xPos = turretSprite.getLocalBounds().width;
    auto yPos = 0.F;

    return turretSprite.getTransform().transformPoint(xPos, yPos);
}

////////////////////////////////////////////////////////////////////////////////
auto Unit::GetRotationPoint() const -> sf::Vector2f {
    auto xPos = 0.F;
    auto yPos = 0.F;

    return turretSprite.getTransform().transformPoint(xPos, yPos);
}

////////////////////////////////////////////////////////////////////////////////
void Unit::SetVelocity(sf::Vector2f newVelocity) {
    velocity = newVelocity;
}

////////////////////////////////////////////////////////////////////////////////
void Unit::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(turretSprite, states);
    target.draw(sprite, states);
}