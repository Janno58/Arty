#pragma once

////////////////////////////////////////////////////////////////////////////////
#include "Pixel.h"
#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////
class Level : public sf::Drawable {
public:

    ////////////////////////////////////////////////////////////////////////////
    Level(unsigned long width, unsigned long height);

    ////////////////////////////////////////////////////////////////////////////
    sf::Vector2u GetSize() const;

    ////////////////////////////////////////////////////////////////////////////
    sf::Color GetPixel(sf::Vector2f pos) const;

    ////////////////////////////////////////////////////////////////////////////
    void SetPixels(const std::vector<Pixel>& newPixels);

private:

    ////////////////////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    ////////////////////////////////////////////////////////////////////////////
    unsigned int width;
    unsigned int height;

    ////////////////////////////////////////////////////////////////////////////
    sf::Texture texture;

    ////////////////////////////////////////////////////////////////////////////
    sf::VertexArray vertices;

    ////////////////////////////////////////////////////////////////////////////
    std::vector<sf::Uint8> pixels;
};

