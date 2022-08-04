#pragma once

////////////////////////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////
class Background : public sf::Drawable {
public:

    ////////////////////////////////////////////////////////////////////////////
    /// \param width Level width pixels
    /// \param height Level height pixels
    ////////////////////////////////////////////////////////////////////////////
    Background(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////////////////////
    /// Background color
    ////////////////////////////////////////////////////////////////////////////
    sf::Color Color;

private:

    ////////////////////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    ////////////////////////////////////////////////////////////////////////////
    sf::Texture texture;

    ////////////////////////////////////////////////////////////////////////////
    sf::VertexArray vertices;

    ////////////////////////////////////////////////////////////////////////////
    sf::Transform transform;

};