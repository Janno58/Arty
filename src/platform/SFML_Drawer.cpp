#include "SFML_Drawer.hpp"
#include "../Config.h"
#include <cassert>
#include <ranges>
#include <cstring>

////////////////////////////////////////////////////////////////////////////////
DrawableTank::DrawableTank(const Player &player, TextureCache& cache) {
    base.setTexture(cache.GetTexture(player.base_texture));
    turret.setTexture(cache.GetTexture(player.turret_texture));

    base.setPosition(player.position);

    turret.setOrigin(TURRET_ORIGIN);
    turret.setPosition(player.position + TURRET_OFFSET);
    turret.setRotation(player.turret_angle + TURRET_ANGLE_OFFSET);
    turret.setScale(TANK_SCALE, TANK_SCALE);

    bottomBar.setFillColor(sf::Color::Red);
    bottomBar.setSize(HEALTHBAR_SIZE);
    topBar.setFillColor(sf::Color::Green);
    topBar.setSize(HEALTHBAR_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
void DrawableTank::Update(const Player &player) {
    base.setPosition(player.position);
    turret.setPosition(player.position + TURRET_OFFSET);
    turret.setRotation(player.turret_angle + TURRET_ANGLE_OFFSET);

    bottomBar.setPosition(player.position + HEALTHBAR_OFFSET);
    topBar.setPosition(player.position + HEALTHBAR_OFFSET);

    const auto health = std::clamp(player.health, 0.F, DEFAULT_STARTING_HEALTH);
    const auto percentage = DEFAULT_STARTING_HEALTH / 100.F * health;
    const auto new_width = bottomBar.getSize().x / 100.F * percentage;
    auto bar_size = topBar.getSize();
    bar_size.x = new_width;

    topBar.setSize(bar_size);
}

////////////////////////////////////////////////////////////////////////////////
void DrawableTank::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(turret, states);
    target.draw(base, states);
    target.draw(bottomBar, states);
    target.draw(topBar, states);
}

////////////////////////////////////////////////////////////////////////////////
SFML_Drawer::SFML_Drawer() {
    constexpr auto tanks_size = 8UL;
    tanks.reserve(tanks_size);
}

////////////////////////////////////////////////////////////////////////////////
void SFML_Drawer::CreateTanks(std::vector<Player> &players) {
    for(auto& tank : players) {
        tanks.emplace_back(tank, textureCache);
    }
}

////////////////////////////////////////////////////////////////////////////////
void SFML_Drawer::UpdateTanks(std::vector<Player> &players) {
    assert(players.size() == tanks.size());

    for(std::tuple<const Player&, DrawableTank&> pair : std::views::zip(players, tanks)) {
        std::get<1>(pair).Update(std::get<0>(pair));
    }
}

////////////////////////////////////////////////////////////////////////////////
void SFML_Drawer::CreateProjectile(const Projectile &shell) {
    projectile.setTexture(textureCache.GetTexture(shell.texture));
}

////////////////////////////////////////////////////////////////////////////////
void SFML_Drawer::UpdateProjectile(const Projectile &shell) {
    drawProjectile = !shell.has_collided;
    projectile.setPosition(shell.position);
    projectile.setRotation(shell.rotation);

}

////////////////////////////////////////////////////////////////////////////////
std::pair<std::vector<uint8_t>, Vec2u> SFML_Drawer::GetTexturePixels(const std::string& textureName) {
    auto tex = textureCache.GetTexture(textureName);
    auto img = tex.copyToImage();
    auto size = Vec2u(img.getSize().x, img.getSize().y);

    std::vector<uint8_t> res(static_cast<unsigned int>(tex.getSize().x * tex.getSize().y * 4), 0);
    std::memcpy(res.data(), img.getPixelsPtr(), static_cast<unsigned int>(img.getSize().x * img.getSize().y *4));

    return {res, size};
}

////////////////////////////////////////////////////////////////////////////////
void SFML_Drawer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for(const auto& tank : tanks) {
        target.draw(tank, states);
    }

    if(drawProjectile) {
        target.draw(projectile, states);
    }
}