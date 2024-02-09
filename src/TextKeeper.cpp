#include "TextKeeper.h"
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
TextKeeper::TextKeeper() {
    if( !font.loadFromFile("SingleDay-Regular.ttf") ) {
        throw std::runtime_error("TextKeeper::TextKeeper: Error loading font!");
    }
}

////////////////////////////////////////////////////////////////////////////////
void TextKeeper::CreateText(const sf::String& str, int size, sf::Vector2f pos) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);

    const auto textBounds = text.getLocalBounds();
    pos.x += textBounds.width / 2;
    pos.y += textBounds.height / 2;
    text.setPosition(pos);

    GameText gText;
    gText.drawable = text;
    gText.timeLeft = sf::seconds(0);
    gText.permanent = true;

    texts.push_back(gText);
}

////////////////////////////////////////////////////////////////////////////////
void TextKeeper::CreateText(const sf::String& str,
                            int size,
                            PresetTxtPos verticalPos,
                            PresetTxtPos horizontalPos) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(size);
    text.setString(str);
    text.setFillColor(sf::Color::Black);

    const auto txtBounds = text.getLocalBounds();
    sf::Vector2f position;

    switch(verticalPos) {
        case PresetTxtPos::FirstFourth:
            position.y = (GUI::POS_Y_FIRST_FOURTH) - (txtBounds.height / 2);
            break;
        case PresetTxtPos::FirstQuarter:
            position.y = (GUI::POS_Y_FIRST_QUARTER) - (txtBounds.height / 2);
            break;
        case PresetTxtPos::Center:
            position.y = (GUI::POS_Y_CENTER) - (txtBounds.height / 2);
            break;
        default:
            break;
    }

    switch(horizontalPos) {
        case PresetTxtPos::Center:
            position.x = (GUI::POS_X_CENTER) - (txtBounds.width / 2);
            break;
        default:
            break;
    }

    text.setPosition(position);

    GameText gTxt;
    gTxt.drawable = text;
    gTxt.permanent = true;
    gTxt.timeLeft = sf::seconds(0);

    texts.push_back(gTxt);
}

////////////////////////////////////////////////////////////////////////////////
void TextKeeper::CreateTimedText(const sf::String& str,
                                 int size,
                                 sf::Time time,
                                 PresetTxtPos verticalPos,
                                 PresetTxtPos horizontalPos) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(size);
    text.setString(str);
    text.setFillColor(sf::Color::Black);

    const auto txtBounds = text.getLocalBounds();
    sf::Vector2f position;

    switch(verticalPos) {
        case PresetTxtPos::FirstFourth:
            position.y = (GUI::POS_Y_FIRST_FOURTH) - (txtBounds.height / 2);
            break;
        case PresetTxtPos::FirstQuarter:
            position.y = (GUI::POS_Y_FIRST_QUARTER) - (txtBounds.height / 2);
            break;
        case PresetTxtPos::Center:
            position.y = (GUI::POS_Y_CENTER) - (txtBounds.height / 2);
            break;
        default:
            break;
    }

    switch(horizontalPos) {
        case PresetTxtPos::Center:
            position.x = (GUI::POS_X_CENTER) - (txtBounds.width / 2);
            break;
        default:
            break;
    }

    text.setPosition(position);

    GameText gTxt;
    gTxt.drawable = text;
    gTxt.permanent = false;
    gTxt.timeLeft = time;

    texts.push_back(gTxt);
}

////////////////////////////////////////////////////////////////////////////////
void TextKeeper::Update() {
    const auto timeElapsed = clock.restart();

    std::erase_if(texts, [&timeElapsed](auto& text) { 
        if(text.permanent) { return false; }

        text.timeLeft -= timeElapsed;
        return text.timeLeft.asMilliseconds() < 0.F;
    });
}

////////////////////////////////////////////////////////////////////////////////
void TextKeeper::Clear() {
    texts.clear();
}

////////////////////////////////////////////////////////////////////////////////
sf::View TextKeeper::View() const {
    return view;
}

////////////////////////////////////////////////////////////////////////////////
void TextKeeper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(const auto& text : texts) {
        target.draw(text.drawable, states);
    }
}

