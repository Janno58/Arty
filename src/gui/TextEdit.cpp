#include "TextEdit.h"

////////////////////////////////////////////////////////////////////////////////
namespace GUI {

////////////////////////////////////////////////////////////////////////////////
TextEdit::TextEdit(sf::String str, sf::Vector2f pos, const sf::Font& font) {
    text.setString(str);
    text.setPosition(pos);
    text.setFont(font);

    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(TEXTEDIT_SIZE);

    globalPos.left = pos.x;
    globalPos.top = pos.y;
}

////////////////////////////////////////////////////////////////////////////////
void TextEdit::DoEvent(const sf::Event& event) {
    if (active && event.type == sf::Event::TextEntered) {
        auto str = text.getString();

        if (event.text.unicode == 0x08) {
            if (str.getSize() != 0) {
                str.erase(str.getSize() - 1);
            }
        } else {
            str += event.text.unicode;
        }

        text.setString(str);
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string TextEdit::GetText() {
    return text.getString();
}

////////////////////////////////////////////////////////////////////////////////
const sf::FloatRect& TextEdit::GetRect() {
    return globalPos;
}


////////////////////////////////////////////////////////////////////////////////
void TextEdit::SetActive(bool state = true) {
    if(state) {
        text.setFillColor(sf::Color::Green);
    } else {
        text.setFillColor(sf::Color::Black);
    }

    active = state;
}

////////////////////////////////////////////////////////////////////////////////
void TextEdit::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(text, states);
}

} /* ns GUI */