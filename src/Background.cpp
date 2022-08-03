#include "Background.h"

////////////////////////////////////////////////////////////////////////////////
Background::Background(unsigned int width, unsigned int height)
: vertices(sf::Quads, 4) {

    if(!texture.loadFromFile("backgroundForest.png")) {
        throw std::runtime_error("Error loading texture!");
    }

    texture.setRepeated(true);

    const auto fWidth = static_cast<float>(width);
    const auto fHeight = static_cast<float>(height);

    vertices[0] = sf::Vector2f(0.F, 0.F);
    vertices[1] = sf::Vector2f(fWidth, 0.F);
    vertices[2] = sf::Vector2f(fWidth, fHeight);
    vertices[3] = sf::Vector2f(0.F, fHeight);

    vertices[0].texCoords = sf::Vector2f(0.F, 0.F);
    vertices[1].texCoords = sf::Vector2f(fWidth, 0.F);
    vertices[2].texCoords = sf::Vector2f(fWidth, fHeight);
    vertices[3].texCoords = sf::Vector2f(0.F, fHeight);

    transform.translate(0.F, -200.F);
}

////////////////////////////////////////////////////////////////////////////////
void Background::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.texture = &texture;
    states.transform = transform;
    target.draw(vertices, states);
}