#include "Level.h"
#include "LevelGen.h"
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
Level::Level(float w, float h)
: width(w), height(h), vertices(sf::Quads, 4), background(w, h) {
    
    assert(w > 0.F && h > 0.F);

    texture.create(static_cast<unsigned int>(w), static_cast<unsigned int>(h));

    vertices[0] = sf::Vector2f(0.F, 0.F);
    vertices[1] = sf::Vector2f(width, 0.F);
    vertices[2] = sf::Vector2f(width, height);
    vertices[3] = sf::Vector2f(0.F, height);

    vertices[0].texCoords = sf::Vector2f(0.F, 0.F);
    vertices[1].texCoords = sf::Vector2f(width, 0.F);
    vertices[2].texCoords = sf::Vector2f(width, height);
    vertices[3].texCoords = sf::Vector2f(0.F, height);

    const auto heights = GenerateWave(static_cast<int>(width));
    pixels = PixelsFromHeights(heights, static_cast<int>(height));
    texture.update(pixels.data());

    spawns.emplace_back(sf::Vector2f(430.F, 0.F));
    spawns.emplace_back(sf::Vector2f(1725.F, 0.F));
}

////////////////////////////////////////////////////////////////////////////////
sf::Color Level::GetBackgroundColor() const {
    return background.Color;
}

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f Level::GetSize() const {
    return {width, height};
}

////////////////////////////////////////////////////////////////////////////////
sf::Color Level::GetPixel(sf::Vector2f pos) const {
    auto xPos = static_cast<unsigned long>(pos.x);
    auto yPos = static_cast<unsigned long>(pos.y);
    auto index = (xPos + yPos * static_cast<unsigned long>(width)) * 4;

    if(index > static_cast<unsigned long>(width * height) * 4) {
        return sf::Color(0,0,0,0);
    }

    sf::Color color;
    color.r = pixels[index];
    color.g = pixels[index+1];
    color.b = pixels[index+2];
    color.a = pixels[index+3];

    return color;
}

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f Level::GetSpawn(std::vector<sf::Vector2f>::size_type index) const {
    return spawns.at(index);
}

////////////////////////////////////////////////////////////////////////////////
void Level::SetPixels(const std::vector<Pixel>& newPixels)
{
    for(const auto& pixel : newPixels) {
        assert(pixel.x >= 0);
        assert(pixel.y >= 0);

        auto xPos = pixel.x;
        auto yPos = pixel.y;
        auto index = (xPos + yPos * width) * 4;

        if(index > width * height * 4) {
            break;
        }

        pixels[index++] = pixel.color.r;
        pixels[index++] = pixel.color.g;
        pixels[index++] = pixel.color.b;
        pixels[index++] = pixel.color.a;
    }

    texture.update(pixels.data());
}

////////////////////////////////////////////////////////////////////////////////
void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(background, states);
    states.texture = &texture;
    target.draw(vertices, states);
}
