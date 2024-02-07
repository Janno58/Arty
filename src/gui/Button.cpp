#include "Button.h"
#include <utility>
#include "GuiFunctions.h"

////////////////////////////////////////////////////////////////////////////////
GUI::Button::Button(std::string ident, sf::Vector2f pos, sf::Vector2f size, const ButtonTextDescriptor& desc)
: identifier(std::move(ident)), position(pos), texture(desc.texture1), textureHover(desc.texture2) {
    vertices = CreateWindowVertices(size, texture->getSize());

    text.setString(desc.text);
    text.setFont(*desc.font);
    text.setCharacterSize(desc.size);
    text.setFillColor(desc.color);

    const auto width = size.x * 0.5F - text.getLocalBounds().width * 0.5F;
    text.setPosition(width, 0);

    boundingBox = sf::FloatRect(pos.x, pos.y, size.x, size.y);
}

////////////////////////////////////////////////////////////////////////////////
void GUI::Button::DoEvent(const sf::Event &event) {
    clicked = hovered && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left;
}

////////////////////////////////////////////////////////////////////////////////
void GUI::Button::MouseMove(sf::Vector2f mousePos) {
    hovered = boundingBox.contains(mousePos);
}

////////////////////////////////////////////////////////////////////////////////
void GUI::Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.texture = hovered ? textureHover : texture;

    (void)states.transform.translate(position);
    target.draw(vertices.data(), vertices.size(), sf::Triangles, states);
    target.draw(text, states);
}
