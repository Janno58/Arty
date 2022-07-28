#include "Unit.h"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
Unit::Unit() {
    if(!texture.loadFromFile("tanks_tankGreen_body3.png")) {
        throw std::runtime_error("Error loading texture!");
    }

    if(!turretTexture.loadFromFile("tanks_turret1.png")) {
        throw std::runtime_error("Error loading texture!");
    }

    sprite.setTexture(texture);
    sprite.scale(scale, scale);

    spriteCenterOffset.x = sprite.getGlobalBounds().width / 2;
    spriteCenterOffset.y = sprite.getGlobalBounds().height / 2;

    turretSprite.setTexture(turretTexture);
    turretSprite.scale(scale, scale);
    turretSprite.setOrigin(0.F, turretSprite.getLocalBounds().height / 2);
    turretOffset = sf::Vector2f(sprite.getGlobalBounds().width / 2, 5.F);

    SetPosition(sf::Vector2f(430.F, 0.F));
}

void Unit::MouseMove(sf::Vector2f mousePos) {
    auto unitVec = sprite.getPosition() + spriteCenterOffset - mousePos;

    auto angle = std::atan2(unitVec.y, unitVec.x) * 180.F / M_PI;
    turretSprite.setRotation(angle + 180.F);
}


////////////////////////////////////////////////////////////////////////////////
void Unit::StepPhysics(float dt) {
    velocity += (Gravity / inverseMass) * dt;
    auto pos = sprite.getPosition();
    SetPosition(pos + (velocity * dt));
}

////////////////////////////////////////////////////////////////////////////////
auto Unit::GetPosition() const -> sf::Vector2f {
    return sprite.getPosition();
}

////////////////////////////////////////////////////////////////////////////////
void Unit::SetPosition(sf::Vector2f newPos)
{
    sprite.setPosition(newPos);
    turretSprite.setPosition(newPos + turretOffset);
}

////////////////////////////////////////////////////////////////////////////////
auto Unit::GetSize() const -> sf::Vector2f {
    auto width = static_cast<float>(texture.getSize().x);
    auto height = static_cast<float>(texture.getSize().y);

    return sf::Vector2f(width, height) * scale;
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