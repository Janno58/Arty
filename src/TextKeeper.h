#pragma once

////////////////////////////////////////////////////////////////////////////////
#include "gui/Config.h"
#include <SFML/Graphics.hpp>
#include <list>

////////////////////////////////////////////////////////////////////////////////
enum class PresetTxtPos {
    FirstFourth,
    FirstQuarter,
    Center
};

////////////////////////////////////////////////////////////////////////////////
struct GameText {

    ////////////////////////////////////////////////////////////////////////////
    sf::Text drawable;   

    ////////////////////////////////////////////////////////////////////////////
    sf::Time timeLeft;
    
    ////////////////////////////////////////////////////////////////////////////
    bool permanent{true}; 
};

////////////////////////////////////////////////////////////////////////////////
/// Class TextKeeper stores and draws all text
/// Text is drawn in a view with size viewPortSize and gets scaled by the game
/// window size
////////////////////////////////////////////////////////////////////////////////
class TextKeeper : public sf::Drawable {
public:
    
    ////////////////////////////////////////////////////////////////////////////
    TextKeeper();

    ////////////////////////////////////////////////////////////////////////////
    void CreateText(const sf::String& str, int size, sf::Vector2f pos);

    ////////////////////////////////////////////////////////////////////////////
    void CreateText(const sf::String& str,
                         int size,
                         PresetTxtPos verticalPos,
                         PresetTxtPos horizontalPos);

    ////////////////////////////////////////////////////////////////////////////
    void CreateTimedText(const sf::String& str,
                         int size,
                         sf::Time time,
                         PresetTxtPos verticalPos,
                         PresetTxtPos horizontalPos);

    ////////////////////////////////////////////////////////////////////////////
    void Update();

    ////////////////////////////////////////////////////////////////////////////
    void Clear();

    ////////////////////////////////////////////////////////////////////////////
    sf::View View() const;

private:
    
    ////////////////////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    ////////////////////////////////////////////////////////////////////////////
    sf::Font font;
    
    ////////////////////////////////////////////////////////////////////////////
    sf::Clock clock;

    ////////////////////////////////////////////////////////////////////////////
    sf::FloatRect viewPortSize{0.F, 0.F, GUI::VIEWPORT_WIDTH, GUI::VIEWPORT_HEIGHT};
 
    ////////////////////////////////////////////////////////////////////////////
    sf::View view{viewPortSize};

    ////////////////////////////////////////////////////////////////////////////
    std::list<GameText> texts;

};