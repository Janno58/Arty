#include "Background.h"

////////////////////////////////////////////////////////////////////////////////
void Background::Create(float width, float height) {
    if(!texture.loadFromFile("backgroundForest.png")) {
        throw std::runtime_error("Error loading texture!");
    }

    texture.setRepeated(true);

    vertices[0] = sf::Vector2f(0.F, 0.F);
    vertices[1] = sf::Vector2f(width, 0.F);
    vertices[2] = sf::Vector2f(width, height);
    vertices[3] = sf::Vector2f(0.F, height);

    vertices[0].texCoords = sf::Vector2f(0.F, 0.F);
    vertices[1].texCoords = sf::Vector2f(width, 0.F);
    vertices[2].texCoords = sf::Vector2f(width, height);
    vertices[3].texCoords = sf::Vector2f(0.F, height);

    const sf::Vector2f offset(0.F, -200.F);
    transform.translate(offset);

    valid = true;
}

////////////////////////////////////////////////////////////////////////////////
void Background::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if(!valid) { return; }

    states.texture = &texture;
    states.transform = transform;
    target.draw(vertices, states);
}