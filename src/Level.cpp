#include "Level.h"
#include "LevelGen.h"

////////////////////////////////////////////////////////////////////////////////
Level::Level(float w, float h) : width(w), height(h), vertices(sf::Quads, 4) {
    texture.create(static_cast<unsigned int>(w), static_cast<unsigned int>(h));

    vertices[0] = sf::Vector2f(0.F, 0.F);
    vertices[1] = sf::Vector2f(w, 0.F);
    vertices[2] = sf::Vector2f(w, h);
    vertices[3] = sf::Vector2f(0.F, h);

    vertices[0].texCoords = sf::Vector2f(0.F, 0.F);
    vertices[1].texCoords = sf::Vector2f(w, 0.F);
    vertices[2].texCoords = sf::Vector2f(w, h);
    vertices[3].texCoords = sf::Vector2f(0.F, h);

    const auto heights = GenerateWave(static_cast<int>(width));
    pixels = PixelsFromHeights(heights, static_cast<int>(height));
    texture.update(pixels.data());
}

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f Level::GetSize() const {
    return {width, height};
}

////////////////////////////////////////////////////////////////////////////////
void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.texture = &texture;
    target.draw(vertices, states);
}